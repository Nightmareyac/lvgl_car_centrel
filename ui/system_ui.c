
#include "system_ui.h"
#include "screens.h"
#include "ui.h"
#include "music.h"

#include <stdio.h>

//sysfs 背光控制文件路径
#define BACKLIGHT_PATH "/sys/devices/platform/backlight/backlight/backlight/brightness"

//音量滑块范围
#define VOLUME_MIN 0
#define VOLUME_MAX 100

static void mapin_btn_cb(lv_event_t * e)
{
    lv_event_code_t code = lv_event_get_code(e);
    if(code == LV_EVENT_CLICKED) {
        loadScreen(SCREEN_ID_MAP);
    }
}


// 设置屏幕亮度（通过 sysfs 写入背光值）
static void set_brighness(int value)
{
    FILE * fp = fopen(BACKLIGHT_PATH, "w");
    if(fp != NULL) {
        fprintf(fp, "%d", value);
        fclose(fp);
    } else {
        printf("cannot open brightness file\n");
    }
}

//音量按钮回调 — 切换音量滑块的显示/隐藏
static void sound_btn_cb(lv_event_t * e)
{
    if(lv_obj_has_state(objects.sound_btn, LV_STATE_CHECKED)) {
        lv_obj_clear_flag(objects.sound_bar, LV_OBJ_FLAG_HIDDEN);
    } else {
        lv_obj_add_flag(objects.sound_bar, LV_OBJ_FLAG_HIDDEN);
    }
}

// 音量滑块变化回调 — 将滑块值传递给底层音乐播放器
void sound_bar_cb(lv_event_t * e)
{
    if(!objects.sound_bar) return;

    int value = lv_slider_get_value(objects.sound_bar);
    music_set_volume(value);
    printf("[sound volume] set value to: %d\n", value);
}

//亮度滑块变化回调 — 将滑块值写入 sysfs 背光文件
void brightness_slider_cb(lv_event_t * e)
{
    if(!objects.brightness_bar) return;

    int value = lv_slider_get_value(objects.brightness_bar);
    set_brighness(value);
    printf("[brightness] set value to: %d\n", value);
}

// 亮度按钮回调 — 切换亮度滑块的显示/隐藏
void brightness_btn_cb(lv_event_t * e)
{
    if(lv_obj_has_state(objects.brightness_btn, LV_STATE_CHECKED)) {
        lv_obj_clear_flag(objects.brightness_bar, LV_OBJ_FLAG_HIDDEN);
    } else {
        lv_obj_add_flag(objects.brightness_bar, LV_OBJ_FLAG_HIDDEN);
    }
}

// 初始化系统设置 UI 模块
void system_ui_init(void)
{
    if(objects.mapin_btn) {
        lv_obj_add_event_cb(objects.mapin_btn, mapin_btn_cb, LV_EVENT_CLICKED, NULL);
    }

    // 音量控制初始化
    if(objects.sound_bar) {
        lv_obj_add_flag(objects.sound_bar, LV_OBJ_FLAG_HIDDEN); /* 默认隐藏 */
        lv_slider_set_range(objects.sound_bar, VOLUME_MIN, VOLUME_MAX);
        lv_obj_add_event_cb(objects.sound_bar, sound_bar_cb, LV_EVENT_VALUE_CHANGED, NULL);
    }
    if(objects.sound_btn) {
        lv_obj_add_flag(objects.sound_btn, LV_OBJ_FLAG_CHECKABLE); /* 可切换状态 */
        lv_obj_add_event_cb(objects.sound_btn, sound_btn_cb, LV_EVENT_CLICKED, NULL);
    }

    // 亮度控制初始化
    if(objects.brightness_bar) {
        lv_obj_add_flag(objects.brightness_bar, LV_OBJ_FLAG_HIDDEN); /* 默认隐藏 */
        lv_obj_add_event_cb(objects.brightness_bar, brightness_slider_cb, LV_EVENT_VALUE_CHANGED, NULL);
    }
    if(objects.brightness_btn) {
        lv_obj_add_event_cb(objects.brightness_btn, brightness_btn_cb, LV_EVENT_CLICKED, NULL);
    }
}
