#include "timePanel.h"
#include <curl/curl.h>
#include <stdlib.h>
#include <string.h>
#include "cJSON.h"
#include "screens.h"

//获取天气icon地址
static const char * get_weather_icon(const char * wea)
{
    if(!wea) {
        return "S:/userdata/assets/cloud.bmp";
    }
    if(strstr(wea, "晴")) {
        return "S:/userdata/assets/sun.bmp";
    }
    if(strstr(wea, "雨")) {
        return "S:/userdata/assets/rain.bmp";
    }
    if(strstr(wea, "雪")) {
        return "S:/userdata/assets/snow.bmp";
    }
    return "S:/userdata/assets/cloud.bmp";
}

// 中文映射转英文
static const char * translate_weather(const char * wea)
{
    if(!wea) return "Cloudy";
    if(strstr(wea, "晴")) return "Sun";
    if(strstr(wea, "多云")) return "Cloudy";
    if(strstr(wea, "阴")) return "Cloudy";
    if(strstr(wea, "小雨")) return "Rainy";
    if(strstr(wea, "中雨")) return "Rainy";
    if(strstr(wea, "大雨") || strstr(wea, "暴雨")) return "Rainy";
    if(strstr(wea, "雨")) return "Rainy";
    return wea; // 其他情况原样返回
}

//更新天气ui
void update_weather_ui(const WealthInfo * info)
{
    if(!info) 
    {
        return;
    }

    lv_obj_t * label[3] = {objects.day1label, objects.day2label, objects.day3label};
    lv_obj_t * icons[3] = {objects.day1icon, objects.day2icon, objects.day3icon};


    //文本和ui读取
    for(int i = 0; i < 3; i++) {
        if(label[i]) 
        {
            lv_label_set_text_fmt(label[i], "%s   %s~%s°C",translate_weather(info->days[i].wea),info->days[i].tem2,info->days[i].tem1);
        }
        if(icons[i]) {
            lv_img_set_src(icons[i], get_weather_icon(info->days[i].wea));
        }
    }
}


//CurlBuf 用于保存天气接口返回的数据。
//HTTP 响应可能分多次到达curl 每收到一段数据，就调用一次 write_cb()
//回调函数通过 realloc() 扩大缓冲区，并用 memcpy() 把新数据追加进去。等请求完成后，fetch_weather_date() 再把完整 JSON 解析成天气结构体，最后交给 UI 模块显示。
typedef struct
{
    char * memory;
    size_t size;
} CurlBuf;

static size_t write_cb(void * contents, size_t size, size_t nmemb, void * userp)
{
    size_t realsize = size * nmemb;
    CurlBuf * buf   = (CurlBuf *)userp;
    char * ptr      = realloc(buf->memory, buf->size + realsize + 1);
    if(!ptr) return 0;
    buf->memory = ptr;
    memcpy(buf->memory + buf->size, contents, realsize);
    buf->size += realsize;
    buf->memory[buf->size] = '\0';
    return realsize;
}

// 返回 0 成功，-1 失败
// 接口：POST https://sdtqz.market.alicloudapi.com/api/weather/seven/days
#define WEATHER_APPCODE "b75eac8580ed433898011801abc05a21"

int fetch_weather_date(const char * city_name, WealthInfo * out_info)
{
    if(!city_name || !out_info) return -1;

    // POST body: city=武汉
    char postfields[128];
    snprintf(postfields, sizeof(postfields), "city=%s", city_name);

    // curl 请求
    CurlBuf buf = {malloc(1), 0};
    if(!buf.memory) return -1;

    CURL * curl = curl_easy_init();
    if(!curl) {
        free(buf.memory);
        return -1;
    }

    struct curl_slist * headers = NULL;
    headers                     = curl_slist_append(headers, "Authorization: APPCODE " WEATHER_APPCODE);
    headers = curl_slist_append(headers, "Content-Type: application/x-www-form-urlencoded; charset=UTF-8");

    curl_easy_setopt(curl, CURLOPT_URL, "https://sdtqz.market.alicloudapi.com/api/weather/seven/days");
    curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
    curl_easy_setopt(curl, CURLOPT_POSTFIELDS, postfields);
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_cb);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, &buf);
    curl_easy_setopt(curl, CURLOPT_TIMEOUT, 10L);
    curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 0L);

    CURLcode res = curl_easy_perform(curl);
    curl_slist_free_all(headers);
    curl_easy_cleanup(curl);

    if(res != CURLE_OK) {
        printf("[WEATHER] curl error: %s\n", curl_easy_strerror(res));
        free(buf.memory);
        return -1;
    }

    // 解析 JSON：root -> data -> data -> { city, data[] }
    cJSON * root = cJSON_Parse(buf.memory);
    free(buf.memory);
    if(!root) {
        printf("[WEATHER] JSON parse failed\n");
        return -1;
    }

    cJSON * outer = cJSON_GetObjectItem(root, "data");
    cJSON * inner = outer ? cJSON_GetObjectItem(outer, "data") : NULL;
    if(!inner) {
        cJSON_Delete(root);
        return -1;
    }

    // city
    cJSON * city = cJSON_GetObjectItem(inner, "city");
    if(city && city->valuestring)
        snprintf(out_info->city, sizeof(out_info->city), "%s", city->valuestring);
    else
        snprintf(out_info->city, sizeof(out_info->city), "%s", city_name);

    // data 数组（取前3天）
    cJSON * data = cJSON_GetObjectItem(inner, "data");
    if(!data || !cJSON_IsArray(data)) {
        cJSON_Delete(root);
        return -1;
    }

    int count = cJSON_GetArraySize(data);
    if(count > 3) count = 3;
    memset(out_info->days, 0, sizeof(out_info->days));

    for(int i = 0; i < count; i++) {
        cJSON * day  = cJSON_GetArrayItem(data, i);
        cJSON * date = cJSON_GetObjectItem(day, "date");
        cJSON * wea  = cJSON_GetObjectItem(day, "wea");
        cJSON * tem1 = cJSON_GetObjectItem(day, "tem1"); // 最高温
        cJSON * tem2 = cJSON_GetObjectItem(day, "tem2"); // 最低温

        if(date && date->valuestring)
            snprintf(out_info->days[i].date, sizeof(out_info->days[i].date), "%s", date->valuestring);
        if(wea && wea->valuestring)
            snprintf(out_info->days[i].wea, sizeof(out_info->days[i].wea), "%s", wea->valuestring);
        if(tem1 && tem1->valuestring)
            snprintf(out_info->days[i].tem1, sizeof(out_info->days[i].tem1), "%s", tem1->valuestring);
        if(tem2 && tem2->valuestring)
            snprintf(out_info->days[i].tem2, sizeof(out_info->days[i].tem2), "%s", tem2->valuestring);
    }

    cJSON_Delete(root);
    return 0;
}

void * weather_thread_func(void * arg)
{
    WealthInfo info;
    if(fetch_weather_date("武汉", &info) == 0) 
    {
        update_weather_ui(&info);
    }
    return NULL;
}