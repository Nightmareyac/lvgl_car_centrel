#ifndef UI_WEATHER_UI_H
#define UI_WEATHER_UI_H

#include <lvgl/lvgl.h>

#ifdef __cplusplus
extern "C" {
#endif

// 初始化天气面板 UI 模块
void weather_ui_init(void);

// 天气按钮 — 展开天气面板并启动后台线程获取天气数据
void wealth_btn_cb(lv_event_t *e);

// 日历按钮 — 切换日历面板的显示/隐藏
void calander_btn_cb(lv_event_t *e);

#ifdef __cplusplus
}
#endif

#endif /* UI_WEATHER_UI_H */
