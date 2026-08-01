#include "screens.h"
#include <lvgl/lvgl.h>
#include <curl/curl.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "cJSON.h"
#include "ui.h"

// 解码库
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#define APIHZ_ID "10019553"
#define APIHZ_KEY "96c6ae926311bf2fd4ae9cae041f05a6"

// 经纬度与尺寸设置
#define MAP_LNG "114.421439" // 经度
#define MAP_LAT "30.467448"  // 纬度
#define MAP_WIDTH "677"
#define MAP_HEIGHT "600"
#define MAP_ZOOM "15"

// 文件路径
#define SYS_JSON_PATH "/userdata/ip_map.json"
#define SYS_PNG_PATH "/userdata/ip_map.png"
#define SYS_BMP_PATH "/userdata/ip_map.bmp"
#define LVGL_BMP_PATH "S:/userdata/ip_map.bmp"

static pthread_t g_ip_map_thread;
static int base64_value(char c)
{
    if(c >= 'A' && c <= 'Z') return c - 'A';
    if(c >= 'a' && c <= 'z') return c - 'a' + 26;
    if(c >= '0' && c <= '9') return c - '0' + 52;
    if(c == '+') return 62;
    if(c == '/') return 63;
    return -1;
}

static unsigned char * decode_base64(const char * src, size_t * out_size)
{
    size_t src_len      = strlen(src);
    size_t capacity     = (src_len * 3) / 4 + 3;
    unsigned char * out = malloc(capacity);
    int accumulator     = 0;
    int bits            = -8;
    size_t written      = 0;

    if(!out) return NULL;

    for(size_t i = 0; i < src_len; i++) {
        int value;
        if(src[i] == '=' || src[i] == '\r' || src[i] == '\n' || src[i] == ' ' || src[i] == '\t') {
            continue;
        }
        value = base64_value(src[i]);
        if(value < 0) {
            free(out);
            return NULL;
        }
        accumulator = (accumulator << 6) | value;
        bits += 6;
        if(bits >= 0) {
            out[written++] = (unsigned char)((accumulator >> bits) & 0xFF);
            bits -= 8;
        }
    }

    *out_size = written;
    return out;
}

static int decode_api_response_to_png(const char * json_path, const char * png_path)
{
    FILE * f = fopen(json_path, "rb");
    long file_size;
    char * json_text;
    cJSON * root;
    cJSON * code;
    cJSON * msg;
    unsigned char * png_data;
    size_t png_size;
    FILE * png_file;
    int result = -1;

    if(!f) return -1;
    if(fseek(f, 0, SEEK_END) != 0) {
        fclose(f);
        return -1;
    }
    file_size = ftell(f);
    if(file_size <= 0 || fseek(f, 0, SEEK_SET) != 0) {
        fclose(f);
        return -1;
    }

    json_text = malloc((size_t)file_size + 1);
    if(!json_text) {
        fclose(f);
        return -1;
    }
    if(fread(json_text, 1, (size_t)file_size, f) != (size_t)file_size) {
        fclose(f);
        free(json_text);
        return -1;
    }
    fclose(f);
    json_text[file_size] = '\0';

    root = cJSON_Parse(json_text);
    free(json_text);
    if(!root) {
        printf("[IP_MAP] API 返回内容不是有效 JSON\n");
        return -1;
    }

    code = cJSON_GetObjectItemCaseSensitive(root, "code");
    msg  = cJSON_GetObjectItemCaseSensitive(root, "msg");
    if(!cJSON_IsNumber(code) || !cJSON_IsString(msg) || code->valueint != 200) {
        printf("[IP_MAP] API 返回失败: code=%d, msg=%s\n", cJSON_IsNumber(code) ? code->valueint : -1,
               cJSON_IsString(msg) ? msg->valuestring : "unknown");
        cJSON_Delete(root);
        return -1;
    }

    png_data = decode_base64(msg->valuestring, &png_size);
    if(!png_data || png_size < 8 || png_data[0] != 0x89 || png_data[1] != 0x50 || png_data[2] != 0x4E ||
       png_data[3] != 0x47 || png_data[4] != 0x0D || png_data[5] != 0x0A || png_data[6] != 0x1A ||
       png_data[7] != 0x0A) {
        printf("[IP_MAP] API msg 不是有效 PNG Base64 数据\n");
        free(png_data);
        cJSON_Delete(root);
        return -1;
    }

    png_file = fopen(png_path, "wb");
    if(png_file) {
        result = (fwrite(png_data, 1, png_size, png_file) == png_size) ? 0 : -1;
        fclose(png_file);
    }

    free(png_data);
    cJSON_Delete(root);
    return result;
}

/* Convert the API's PNG response to an LVGL-compatible 24-bit BMP. */
static int convert_image_to_bmp(const char * image_path, const char * bmp_path)
{
    FILE * image = fopen(image_path, "rb");
    unsigned char header[16];
    size_t header_size;

    if(!image) {
        printf("[IP_MAP] 无法打开下载文件进行校验\n");
        return -1;
    }

    header_size = fread(header, 1, sizeof(header), image);
    fclose(image);

    if(header_size < 8 || header[0] != 0x89 || header[1] != 0x50 || header[2] != 0x4E || header[3] != 0x47 ||
       header[4] != 0x0D || header[5] != 0x0A || header[6] != 0x1A || header[7] != 0x0A) {
        printf("[IP_MAP] 下载内容不是 PNG，文件头:");
        for(size_t i = 0; i < header_size; i++) {
            printf(" %02X", header[i]);
        }
        printf("\n");
        return -1;
    }

    int width, height, channels;
    unsigned char * img = stbi_load(image_path, &width, &height, &channels, 3);
    if(!img) {
        printf("[IP_MAP] stb_image 解析失败: %s\n", stbi_failure_reason());
        return -1;
    }

    FILE * f = fopen(bmp_path, "wb");
    if(!f) {
        stbi_image_free(img);
        return -1;
    }

    const int row_bytes                = 3 * width;
    const int row_stride               = (row_bytes + 3) & ~3;
    const int filesize                 = 54 + row_stride * height;
    const unsigned char row_padding[3] = {0, 0, 0};
    unsigned char bmpfileheader[14]    = {'B', 'M', 0, 0, 0, 0, 0, 0, 0, 0, 54, 0, 0, 0};
    unsigned char bmpinfoheader[40]    = {40, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 24, 0};

    bmpfileheader[2] = (unsigned char)(filesize);
    bmpfileheader[3] = (unsigned char)(filesize >> 8);
    bmpfileheader[4] = (unsigned char)(filesize >> 16);
    bmpfileheader[5] = (unsigned char)(filesize >> 24);

    bmpinfoheader[4]  = (unsigned char)(width);
    bmpinfoheader[5]  = (unsigned char)(width >> 8);
    bmpinfoheader[6]  = (unsigned char)(width >> 16);
    bmpinfoheader[7]  = (unsigned char)(width >> 24);
    bmpinfoheader[8]  = (unsigned char)(height);
    bmpinfoheader[9]  = (unsigned char)(height >> 8);
    bmpinfoheader[10] = (unsigned char)(height >> 16);
    bmpinfoheader[11] = (unsigned char)(height >> 24);

    bmpinfoheader[20] = (unsigned char)(row_stride * height);
    bmpinfoheader[21] = (unsigned char)((row_stride * height) >> 8);
    bmpinfoheader[22] = (unsigned char)((row_stride * height) >> 16);
    bmpinfoheader[23] = (unsigned char)((row_stride * height) >> 24);

    fwrite(bmpfileheader, 1, 14, f);
    fwrite(bmpinfoheader, 1, 40, f);

    for(int i = height - 1; i >= 0; i--) {
        for(int j = 0; j < width; j++) {
            int p                = (i * width + j) * 3;
            unsigned char bgr[3] = {img[p + 2], img[p + 1], img[p]};
            fwrite(bgr, 1, 3, f);
        }
        fwrite(row_padding, 1, (size_t)(row_stride - row_bytes), f);
    }

    fclose(f);
    stbi_image_free(img);
    return 0;
}

static size_t write_file_cb(void * ptr, size_t size, size_t nmemb, void * stream)
{
    return fwrite(ptr, size, nmemb, (FILE *)stream);
}

static void update_map_ui_cb(void * param)
{
    char * path = (char *)param;
    if(!path) return;

    if(objects.map_img) {
        lv_img_set_src(objects.map_img, path);
        lv_obj_set_width(objects.map_img, 677);
        lv_obj_set_height(objects.map_img, 600);
        lv_img_set_zoom(objects.map_img, 256);

        lv_obj_clear_flag(objects.map_img, LV_OBJ_FLAG_HIDDEN);
        lv_obj_move_background(objects.map_img);
        if (objects.exit_map_btn)
            lv_obj_move_foreground(objects.exit_map_btn);
        lv_obj_invalidate(objects.map_img);
    }

    free(path);
}

static void * ip_map_download_thread(void * arg)
{
    (void)arg;
    CURL * curl;
    CURLcode res;
    long http_code          = 0;
    char * content_type_ptr = NULL;
    char content_type[128]  = "unknown";

    char url[512];
    snprintf(url, sizeof(url),
             "https://cn.apihz.cn/api/other/imgtiandi.php?id=%s&key=%s&lng=%s&lat=%s&zoom=%s&width=%s&height=%s",
             APIHZ_ID, APIHZ_KEY, MAP_LNG, MAP_LAT, MAP_ZOOM, MAP_WIDTH, MAP_HEIGHT);

    FILE * fp = fopen(SYS_JSON_PATH, "wb");
    if(!fp) return NULL;

    curl = curl_easy_init();
    if(curl) {
        curl_easy_setopt(curl, CURLOPT_URL, url);
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_file_cb);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, fp);
        curl_easy_setopt(curl, CURLOPT_TIMEOUT, 15L);
        curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 0L);
        curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);
        curl_easy_setopt(curl, CURLOPT_ACCEPT_ENCODING, "");

        printf("[IP_MAP] 正在请求地图 API...\n");
        res = curl_easy_perform(curl);
        curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &http_code);
        curl_easy_getinfo(curl, CURLINFO_CONTENT_TYPE, &content_type_ptr);
        if(content_type_ptr) {
            snprintf(content_type, sizeof(content_type), "%s", content_type_ptr);
        }
        curl_easy_cleanup(curl);
        fclose(fp);

        printf("[IP_MAP] HTTP 状态=%ld, Content-Type=%s\n", http_code, content_type);

        if(res == CURLE_OK && http_code >= 200 && http_code < 300) {
            printf("[IP_MAP] API 响应成功，解析 Base64 PNG...\n");
            if(decode_api_response_to_png(SYS_JSON_PATH, SYS_PNG_PATH) == 0 &&
               convert_image_to_bmp(SYS_PNG_PATH, SYS_BMP_PATH) == 0) {
                char * path_copy = strdup(LVGL_BMP_PATH);
                if(path_copy) {
                    lv_async_call(update_map_ui_cb, path_copy);
                }
            } else {
                printf("[IP_MAP] 图片转换失败！\n");
            }
        } else {
            printf("[IP_MAP] 地图请求失败: %s\n", curl_easy_strerror(res));
        }
    } else {
        fclose(fp);
    }
    return NULL;
}

static void map_exit_btn_click(lv_event_t * e)
{
    lv_event_code_t code = lv_event_get_code(e);
    if(code == LV_EVENT_CLICKED) {
        if(objects.exit_map_btn) {
            loadScreen(SCREEN_ID_MAIN);
        }
    }
}

void load_ip_map_async(void)
{
    pthread_create(&g_ip_map_thread, NULL, ip_map_download_thread, NULL);
    pthread_detach(g_ip_map_thread);

    if (objects.exit_map_btn)
    {
        lv_obj_add_event_cb(objects.exit_map_btn,map_exit_btn_click,LV_EVENT_CLICKED,NULL);
    }
    
}
