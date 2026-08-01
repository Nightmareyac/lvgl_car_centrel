#include "screens.h"
#include "aichat.h"
#include "cJSON.h"

#include <lvgl/lvgl.h>
#include <curl/curl.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

//API 配置
#define API_URL    "https://api.longcat.chat/openai/v1/chat/completions"
#define API_KEY    "ak_2Bv2cl1YL4HG57Z8Dj4l907B1rJ2A"
#define MODEL_NAME "LongCat-2.0"

//单条消息最大字符数
#define MAX_MSG_LEN 2048
//curl 响应数据缓冲区
typedef struct {
    char   *memory;
    size_t  size;
} CurlBuf;

// 待发送消息（由ui回调填充，由后台线程使用） 
static char g_pending_input[MAX_MSG_LEN] = {0};
static int  g_pending_ready = 0;           /**< 0=无待发送，1=有待发送 */
static pthread_mutex_t g_pending_mutex = PTHREAD_MUTEX_INITIALIZER;
static pthread_cond_t  g_pending_cond  = PTHREAD_COND_INITIALIZER;

// 后台网络线程
static pthread_t g_ai_thread;
static int       g_ai_thread_running = 1;


static size_t ai_curl_write_cb(void *contents, size_t size, size_t nmemb, void *userp);
static int    ai_send_request(const char *prompt, char *out_response, size_t buf_size);
static const char *ai_parse_response(const char *json_str);
static void  *ai_thread_func(void *arg);

// curl 写回调
//libcurl 写回调 — 将响应数据追加到 CurlBuf
static size_t ai_curl_write_cb(void *contents, size_t size, size_t nmemb, void *userp)
{
    size_t realsize = size * nmemb;
    CurlBuf *mem = (CurlBuf *)userp;

    char *ptr = realloc(mem->memory, mem->size + realsize + 1);
    if (!ptr) return 0;

    mem->memory = ptr;
    memcpy(&(mem->memory[mem->size]), contents, realsize);
    mem->size += realsize;
    mem->memory[mem->size] = '\0';
    return realsize;
}

//解析 OpenAI 格式的 JSON 响应，提取 AI 回复文本
static const char *ai_parse_response(const char *json_str)
{
    static char content_buf[MAX_MSG_LEN];

    cJSON *root = cJSON_Parse(json_str);
    if (!root) {
        fprintf(stderr, "[AI] JSON 解析失败\n");
        return NULL;
    }

    //提取 choices[0].message.content
    cJSON *choices = cJSON_GetObjectItem(root, "choices");
    if (cJSON_IsArray(choices) && cJSON_GetArraySize(choices) > 0) {
        cJSON *first_choice = cJSON_GetArrayItem(choices, 0);
        cJSON *message = cJSON_GetObjectItem(first_choice, "message");
        if (message) {
            cJSON *content = cJSON_GetObjectItem(message, "content");
            if (cJSON_IsString(content) && content->valuestring) {
                snprintf(content_buf, sizeof(content_buf), "%s", content->valuestring);
                cJSON_Delete(root);
                return content_buf;
            }
        }
    }

    //检查是否有错误信息
    cJSON *error = cJSON_GetObjectItem(root, "error");
    if (error) {
        cJSON *err_msg = cJSON_GetObjectItem(error, "message");
        if (err_msg) {
            snprintf(content_buf, sizeof(content_buf), "[API 错误] %s", err_msg->valuestring);
            cJSON_Delete(root);
            return content_buf;
        }
    }

    cJSON_Delete(root);
    return NULL;
}

/* curl 网络请求                                                        */
//向ai API 发送单次请求并获取回复
static int ai_send_request(const char *prompt, char *out_response, size_t buf_size)
{
    /* 构造 JSON 请求体 */
    cJSON *root = cJSON_CreateObject();
    cJSON_AddStringToObject(root, "model", MODEL_NAME);

    cJSON *messages = cJSON_CreateArray();
    cJSON *msg = cJSON_CreateObject();
    cJSON_AddStringToObject(msg, "role", "user");
    cJSON_AddStringToObject(msg, "content", prompt);
    cJSON_AddItemToArray(messages, msg);
    cJSON_AddItemToObject(root, "messages", messages);

    char *json_body = cJSON_PrintUnformatted(root);
    cJSON_Delete(root);
    if (!json_body) return -1;

    CurlBuf chunk = { malloc(1), 0 };

    CURL *curl = curl_easy_init();
    if (!curl) {
        free(json_body);
        free(chunk.memory);
        return -1;
    }

    /* 构造 Authorization Header */
    char auth_header[256];
    snprintf(auth_header, sizeof(auth_header), "Authorization: Bearer %s", API_KEY);

    struct curl_slist *headers = NULL;
    headers = curl_slist_append(headers, "Content-Type: application/json");
    headers = curl_slist_append(headers, auth_header);

    curl_easy_setopt(curl, CURLOPT_URL, API_URL);
    curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
    curl_easy_setopt(curl, CURLOPT_POSTFIELDS, json_body);
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, ai_curl_write_cb);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, (void *)&chunk);
    curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);
    curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 0L);
    curl_easy_setopt(curl, CURLOPT_TIMEOUT, 30L);

    printf("[AI] 正在发送请求...\n");
    CURLcode res = curl_easy_perform(curl);

    if (res != CURLE_OK) {
        fprintf(stderr, "[AI] curl 请求失败: %s\n", curl_easy_strerror(res));
        snprintf(out_response, buf_size, "[网络错误] %s", curl_easy_strerror(res));
    } else {
        /* 解析响应获取 AI 回复 */
        const char *ai_text = ai_parse_response(chunk.memory);
        if (ai_text) {
            snprintf(out_response, buf_size, "%s", ai_text);
        } else {
            snprintf(out_response, buf_size, "[错误] 无法解析 API 响应");
        }
    }

    curl_slist_free_all(headers);
    curl_easy_cleanup(curl);
    free(json_body);
    free(chunk.memory);

    return 0;
}

/* ------------------------------------------------------------------ */
/* UI 异步更新（线程安全）                                                */
/* ------------------------------------------------------------------ */

//lv_async_call 回调 — 在主线程中更新 ai_answer 标签文本
static void ai_update_answer_cb(void *param)
{
    char *text = (char *)param;
    if (objects.ai_answer) {
        lv_label_set_text(objects.ai_answer, text);
    }
    free(text);  /* lv_async_call 要求调用者管理内存 */
}

//更新 AI 回复显示（可从任意线程调用）
static void ai_display_response(const char *text)
{
    char *copy = strdup(text);
    if (copy) {
        lv_async_call(ai_update_answer_cb, copy);
    }
}

// 后台网络线程 
// 后台线程函数,循环等待待发送消息，调用 API 并更新 UI
static void *ai_thread_func(void *arg)
{
    (void)arg;

    while (g_ai_thread_running) {
        // 等待用户发送消息（cond_wait 休眠，零 CPU 占用） 
        pthread_mutex_lock(&g_pending_mutex);
        while (!g_pending_ready && g_ai_thread_running) {
            pthread_cond_wait(&g_pending_cond, &g_pending_mutex);
        }
        if (!g_ai_thread_running) {
            pthread_mutex_unlock(&g_pending_mutex);
            break;
        }
        // 取出待发送的消息 
        char input[MAX_MSG_LEN];
        snprintf(input, sizeof(input), "%s", g_pending_input);
        g_pending_ready = 0;
        pthread_mutex_unlock(&g_pending_mutex);

        // 发送 API 请求 
        char response[MAX_MSG_LEN];
        if (ai_send_request(input, response, sizeof(response)) == 0) {
            ai_display_response(response);
        } else {
            ai_display_response("[错误] 请求失败，请检查网络连接");
        }
    }

    return NULL;
}


// LVGL 事件回调 
//发送按钮点击回调
static void ai_send_btn_cb(lv_event_t *e)
{
    if (!objects.ai_userrequest) return;

    // 确保聊天框可见 
    if (objects.ai_chatbox) {
        lv_obj_clear_flag(objects.ai_chatbox, LV_OBJ_FLAG_HIDDEN);
    }

    // 获取用户输入文本 
    const char *text = lv_textarea_get_text(objects.ai_userrequest);
    if (!text || strlen(text) == 0) return;

    if (objects.ai_answer) {
        lv_label_set_text(objects.ai_answer, "Thinking...");
    }

    // 唤醒后台线程发送请求（mutex 保护
    pthread_mutex_lock(&g_pending_mutex);
    snprintf(g_pending_input, sizeof(g_pending_input), "%s", text);
    g_pending_ready = 1;
    pthread_cond_signal(&g_pending_cond);
    pthread_mutex_unlock(&g_pending_mutex);

    //清空输入框，准备下一次输入 
    lv_textarea_set_text(objects.ai_userrequest, "");
}

// 关闭聊天框按钮回调
static void ai_chatbox_shut_cb(lv_event_t *e)
{
    if (objects.ai_chatbox) {
        lv_obj_add_flag(objects.ai_chatbox, LV_OBJ_FLAG_HIDDEN);
    }
}

// 用户输入框点击,弹出键盘并绑定到输入框
static void ai_userrequest_cb(lv_event_t *e)
{
    lv_event_code_t code = lv_event_get_code(e);
    if (code == LV_EVENT_CLICKED || code == LV_EVENT_FOCUSED) {
        if (objects.ai_chatkeyboard && objects.ai_userrequest) {
            lv_keyboard_set_textarea(objects.ai_chatkeyboard, objects.ai_userrequest);
            lv_obj_clear_flag(objects.ai_chatkeyboard, LV_OBJ_FLAG_HIDDEN);
        }
    }
}

// 键盘关闭回调
static void ai_keybord_cb(lv_event_t *e)
{
    lv_event_code_t code = lv_event_get_code(e);
    if (code == LV_EVENT_READY || code == LV_EVENT_CANCEL) {
        if (objects.ai_chatkeyboard) {
            lv_obj_add_flag(objects.ai_chatkeyboard, LV_OBJ_FLAG_HIDDEN);
        }
    }
}

/* 模块初始化                                                           */
//初始化 AI 对话模块
void ai_init(void)
{
    if (objects.ai_userrequest) {
        lv_obj_add_event_cb(objects.ai_userrequest, ai_userrequest_cb, LV_EVENT_ALL, NULL);
    }
    if (objects.ai_chatkeyboard) {
        lv_obj_add_event_cb(objects.ai_chatkeyboard, ai_keybord_cb, LV_EVENT_ALL, NULL);
    }
    if (objects.ai_send_btn) {
        lv_obj_add_event_cb(objects.ai_send_btn, ai_send_btn_cb, LV_EVENT_CLICKED, NULL);
    }
    if (objects.ai_chatbox_shutbtn) {
        lv_obj_add_event_cb(objects.ai_chatbox_shutbtn, ai_chatbox_shut_cb, LV_EVENT_CLICKED, NULL);
    }

    //初始化 curl 全局环境
    curl_global_init(CURL_GLOBAL_ALL);

    //启动后台网络线程
    g_ai_thread_running = 1;
    if (pthread_create(&g_ai_thread, NULL, ai_thread_func, NULL) == 0) {
        pthread_detach(g_ai_thread);  /* 线程结束自动回收资源 */
    } else {
        fprintf(stderr, "[AI] 后台线程创建失败\n");
    }
}
