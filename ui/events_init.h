/**
 * @file    events_init.h
 * @brief   UI 事件统一初始化入口
 * @details 在 ui_init() 之后调用，依次初始化所有业务模块并注册 LVGL 事件回调。
 *          这是替代原来 player_events_init() 的唯一对外接口。
 */

#ifndef UI_EVENTS_INIT_H
#define UI_EVENTS_INIT_H

/* 前向声明类型，避免强制依赖 lvgl.h（调用者通常已包含） */
struct _lv_timer_t;

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief 初始化所有 UI 业务模块并绑定事件回调
 * @details 依次调用各模块的初始化函数：
 *          1. vehicle_init()     — 车辆控制 + 巴法云状态同步定时器
 *          2. player_ui_init()   — 音乐播放器 + 进度条定时器
 *          3. weather_ui_init()  — 天气面板
 *          4. system_ui_init()   — 声音/亮度控制
 */
void ui_events_init(void);

#ifdef __cplusplus
}
#endif

#endif /* UI_EVENTS_INIT_H */
