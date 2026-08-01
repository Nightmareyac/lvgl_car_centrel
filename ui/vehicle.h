
#ifndef UI_VEHICLE_H
#define UI_VEHICLE_H

#include <lvgl/lvgl.h>

#ifdef __cplusplus
extern "C" {
#endif


void vehicle_init(void);

void action_trunk_btn_click(lv_event_t *e);

void action_frunk_btn_click(lv_event_t *e);

void action_lock_btn_click(lv_event_t *e);

void action_flash1_btn_click(lv_event_t *e);

void action_flash2_btn_click(lv_event_t *e);

void action_flash3_btn_click(lv_event_t *e);

void action_airconditionr_call_btn_click(lv_event_t *e);

void bemfa_ui_update_cb(lv_timer_t *timer);

void update_time(void);

#ifdef __cplusplus
}
#endif

#endif /* UI_VEHICLE_H */
