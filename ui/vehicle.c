#include "vehicle.h"
#include "screens.h"
#include "bemfa_client.h"

#include <stdio.h>
#include <time.h>

// 巴法云 MQTT 主题 ID 映射                                             */
#define TOPIC_TRUNK  "mTJT3Afrk006"  // 后备箱 
#define TOPIC_FRUNK  "vc8lsGpsp006"  // 引擎盖
#define TOPIC_LOCK   "DyQL1Jcsc004"  // 车锁 
#define TOPIC_AC     "61SOh2Izr005"  // 空调 
#define TOPIC_LIGHT1 "4lx3WVhUA002"  // 车灯 1 
#define TOPIC_LIGHT2 "6MHL9hbRf002"  // 车灯 2 
#define TOPIC_LIGHT3 "EIHBf8Q8A002"  // 车灯 3 

// 车辆按钮事件回调                                                      */
// 后备箱按钮回调,翻转本地状态并发送 on/off 到巴法云
void action_trunk_btn_click(lv_event_t *e)
{
    static bool open_state = false;
    open_state = !open_state;
    bemfa_send_msg(TOPIC_TRUNK, open_state ? "on" : "off");
}

//引擎盖按钮回调
void action_frunk_btn_click(lv_event_t *e)
{
    static bool open_state = false;
    open_state = !open_state;
    bemfa_send_msg(TOPIC_FRUNK, open_state ? "on" : "off");
}

//车锁按钮回调
void action_lock_btn_click(lv_event_t *e)
{
    static bool is_locked = false;
    is_locked = !is_locked;
    bemfa_send_msg(TOPIC_LOCK, is_locked ? "lock" : "unlock");
}

// 车灯 1 按钮回调 
void action_flash1_btn_click(lv_event_t *e)
{
    static bool light_on = false;
    light_on = !light_on;
    bemfa_send_msg(TOPIC_LIGHT1, light_on ? "on" : "off");
}


// 车灯 2 按钮回调 
 */
void action_flash2_btn_click(lv_event_t *e)
{
    static bool light_on = false;
    light_on = !light_on;
    bemfa_send_msg(TOPIC_LIGHT2, light_on ? "on" : "off");
}


// 车灯 3 按钮回调 
 */
void action_flash3_btn_click(lv_event_t *e)
{
    static bool light_on = false;
    light_on = !light_on;
    bemfa_send_msg(TOPIC_LIGHT3, light_on ? "on" : "off");
}

// 空调按钮回调
void action_airconditionr_call_btn_click(lv_event_t *e)
{
    if (!objects.tem_roller) return;

    lv_obj_t *btn = lv_event_get_target(e);

    if (lv_obj_has_state(btn, LV_STATE_CHECKED)) {
        // 空调开启 — 显示温度滚轮供用户选择 
        lv_obj_clear_flag(objects.tem_roller, LV_OBJ_FLAG_HIDDEN);
    } else {
        // 空调关闭 — 隐藏滚轮并发送选中的温度值 
        lv_obj_add_flag(objects.tem_roller, LV_OBJ_FLAG_HIDDEN);

        uint16_t selected = lv_roller_get_selected(objects.tem_roller);
        char temp_buf[16];
        lv_roller_get_selected_str(objects.tem_roller, temp_buf, sizeof(temp_buf));

        bemfa_send_msg(TOPIC_AC, temp_buf);
        printf("[AC] Temperature set to: %s (index=%d)\n", temp_buf, selected);
    }
}

// 巴法云状态同步定时器                                                  */
// 巴法云 UI 状态同步定时器回调
// 当 g_bemfa_state.is_updated == true 时，将云端下发的各设备状态
void bemfa_ui_update_cb(lv_timer_t *timer)
{
    if (!g_bemfa_state.is_updated) return;

    // 后备箱状态同步 
    if (objects.trunk_btn) {
        if (g_bemfa_state.trunk_open)
            lv_obj_add_state(objects.trunk_btn, LV_STATE_CHECKED);
        else
            lv_obj_clear_state(objects.trunk_btn, LV_STATE_CHECKED);
    }

    // 车锁状态同步 
    if (objects.lock_btn) {
        if (g_bemfa_state.lock_locked)
            lv_obj_add_state(objects.lock_btn, LV_STATE_CHECKED);
        else
            lv_obj_clear_state(objects.lock_btn, LV_STATE_CHECKED);
    }

    // 空调状态同步 
    if (objects.airconditionr_call_btn) {
        if (g_bemfa_state.ac_on)
            lv_obj_add_state(objects.airconditionr_call_btn, LV_STATE_CHECKED);
        else
            lv_obj_clear_state(objects.airconditionr_call_btn, LV_STATE_CHECKED);
    }

    // 引擎盖状态同步 
    if (objects.funk_btn) {
        if (g_bemfa_state.frunk_open)
            lv_obj_add_state(objects.funk_btn, LV_STATE_CHECKED);
        else
            lv_obj_clear_state(objects.funk_btn, LV_STATE_CHECKED);
    }

    // 车灯 1/2/3 状态同步 
    if (objects.flash1) {
        if (g_bemfa_state.flash1_turn)
            lv_obj_add_state(objects.flash1, LV_STATE_CHECKED);
        else
            lv_obj_clear_state(objects.flash1, LV_STATE_CHECKED);
    }
    if (objects.flash2) {
        if (g_bemfa_state.flash2_turn)
            lv_obj_add_state(objects.flash2, LV_STATE_CHECKED);
        else
            lv_obj_clear_state(objects.flash2, LV_STATE_CHECKED);
    }
    if (objects.flash3) {
        if (g_bemfa_state.flash3_turn)
            lv_obj_add_state(objects.flash3, LV_STATE_CHECKED);
        else
            lv_obj_clear_state(objects.flash3, LV_STATE_CHECKED);
    }

    // 标记已处理，等待下一轮云端更新 
    g_bemfa_state.is_updated = false;
}

 // 时间更新定时器回调（每秒执行一次）
static void time_update_timer_cb(lv_timer_t *timer)
{
    time_t now = time(NULL);
    struct tm tm;
    localtime_r(&now, &tm);

    lv_label_set_text_fmt(objects.time_label, "%02d:%02d", tm.tm_hour, tm.tm_min);
    lv_label_set_text_fmt(objects.year_label, "%04d/%02d/%02d",
                          tm.tm_year + 1900, tm.tm_mon + 1, tm.tm_mday);
}

// 模块初始化                   
//初始化车辆控制模块
void vehicle_init(void)
{
    //连接巴法云服务器
    bemfa_connect();

    //创建状态同步定时器（每 100ms 检查云端状态
    lv_timer_create(bemfa_ui_update_cb, 100, NULL);

    // 创建时间更新定时器（每秒刷新一次时:分和年/月/日） */
    lv_timer_create(time_update_timer_cb, 1000, NULL);

    // 绑定车辆控制按钮事件
    if (objects.trunk_btn)
        lv_obj_add_event_cb(objects.trunk_btn, action_trunk_btn_click, LV_EVENT_CLICKED, NULL);
    if (objects.funk_btn)
        lv_obj_add_event_cb(objects.funk_btn, action_frunk_btn_click, LV_EVENT_CLICKED, NULL);
    if (objects.lock_btn)
        lv_obj_add_event_cb(objects.lock_btn, action_lock_btn_click, LV_EVENT_CLICKED, NULL);
    if (objects.flash1)
        lv_obj_add_event_cb(objects.flash1, action_flash1_btn_click, LV_EVENT_CLICKED, NULL);
    if (objects.flash2)
        lv_obj_add_event_cb(objects.flash2, action_flash2_btn_click, LV_EVENT_CLICKED, NULL);
    if (objects.flash3)
        lv_obj_add_event_cb(objects.flash3, action_flash3_btn_click, LV_EVENT_CLICKED, NULL);

    /* 空调按钮默认可点击切换状态 */
    if (objects.airconditionr_call_btn) {
        lv_obj_add_flag(objects.airconditionr_call_btn, LV_OBJ_FLAG_CHECKABLE);
        lv_obj_add_event_cb(objects.airconditionr_call_btn, action_airconditionr_call_btn_click,
                            LV_EVENT_CLICKED, NULL);
    }
}
