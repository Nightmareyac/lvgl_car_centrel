/**
 * @file    vehicle.h
 * @brief   车辆设备控制模块
 * @details 通过巴法云（Bemfa）MQTT 协议控制车辆硬件：后备箱、引擎盖、车锁、空调、三组车灯。
 *          同时负责将云端下发的状态同步到 LVGL UI 控件。
 * @note    依赖 bemfa_client.c 提供的底层 MQTT 连接和消息收发。
 */

#ifndef UI_VEHICLE_H
#define UI_VEHICLE_H

#include <lvgl/lvgl.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief 初始化车辆控制模块
 * @details 依次执行：
 *          1. 连接巴法云服务器
 *          2. 创建状态同步定时器（每 100ms 检查云端状态并刷新 UI）
 *          3. 绑定所有车辆控制按钮的事件回调
 */
void vehicle_init(void);

/* ------------------------------------------------------------------ */
/* 各车辆设备按钮的事件回调（由 LVGL 事件系统调用）                         */
/* ------------------------------------------------------------------ */

/** 后备箱按钮 — 发送开/关指令到巴法云 */
void action_trunk_btn_click(lv_event_t *e);

/** 引擎盖（前备箱）按钮 — 发送开/关指令到巴法云 */
void action_frunk_btn_click(lv_event_t *e);

/** 车锁按钮 — 发送上锁/解锁指令到巴法云 */
void action_lock_btn_click(lv_event_t *e);

/** 车灯 1 按钮 — 发送开/关指令到巴法云 */
void action_flash1_btn_click(lv_event_t *e);

/** 车灯 2 按钮 — 发送开/关指令到巴法云 */
void action_flash2_btn_click(lv_event_t *e);

/** 车灯 3 按钮 — 发送开/关指令到巴法云 */
void action_flash3_btn_click(lv_event_t *e);

/** 空调按钮 — 切换空调开关，关闭时发送选中的温度值 */
void action_airconditionr_call_btn_click(lv_event_t *e);

/* ------------------------------------------------------------------ */
/* 状态同步与时间更新                                                    */
/* ------------------------------------------------------------------ */

/**
 * @brief 巴法云状态同步定时器回调（每 100ms 执行一次）
 * @details 检查 g_bemfa_state.is_updated 标志，若云端有新状态则同步到 UI，
 *          同步完成后清除 is_updated 标志。
 */
void bemfa_ui_update_cb(lv_timer_t *timer);

/**
 * @brief 手动触发一次时间显示更新（时:分和年/月/日）
 * @details 时间由 vehicle_init() 创建的每秒定时器自动刷新，
 *          此函数供外部在需要时手动触发一次立即更新。
 */
void update_time(void);

#ifdef __cplusplus
}
#endif

#endif /* UI_VEHICLE_H */
