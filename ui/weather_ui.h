/**
 * @file    weather_ui.h
 * @brief   天气面板 UI 模块
 * @details 负责天气面板的滑入/滑出动画、日历面板切换、以及后台线程获取天气数据。
 *          实际天气数据获取和 JSON 解析由 timePanel.c 模块实现。
 * @note    保留了 EEZ Studio 生成的原始对象名（如 wealth_panel、calander）。
 */

#ifndef UI_WEATHER_UI_H
#define UI_WEATHER_UI_H

#include <lvgl/lvgl.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief 初始化天气面板 UI 模块
 * @details 绑定天气按钮和日历按钮的事件回调，设置默认隐藏状态。
 */
void weather_ui_init(void);

/** 天气按钮 — 展开天气面板并启动后台线程获取天气数据 */
void wealth_btn_cb(lv_event_t *e);

/** 日历按钮 — 切换日历面板的显示/隐藏 */
void calander_btn_cb(lv_event_t *e);

#ifdef __cplusplus
}
#endif

#endif /* UI_WEATHER_UI_H */
