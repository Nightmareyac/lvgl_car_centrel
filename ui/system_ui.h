/**
 * @file    system_ui.h
 * @brief   系统设置 UI 模块
 * @details 负责音量控制（通过 music.c 设置 mplayer 音量）和屏幕亮度控制（通过 sysfs 写入背光）。
 */

#ifndef UI_SYSTEM_UI_H
#define UI_SYSTEM_UI_H

#include <lvgl/lvgl.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief 初始化系统设置 UI 模块
 * @details 绑定音量滑块、亮度滑块及其展开/折叠按钮的事件回调，设置默认隐藏状态。
 */
void system_ui_init(void);

/** 音量滑块变化回调 — 设置 mplayer 音量 */
void sound_bar_cb(lv_event_t *e);

/** 亮度滑块变化回调 — 写入 sysfs 背光值 */
void brightness_slider_cb(lv_event_t *e);

/** 亮度按钮 — 切换亮度滑块的显示/隐藏 */
void brightness_btn_cb(lv_event_t *e);

#ifdef __cplusplus
}
#endif

#endif /* UI_SYSTEM_UI_H */
