#ifndef UI_EVENTS_INIT_H
#define UI_EVENTS_INIT_H

struct _lv_timer_t;

#ifdef __cplusplus
extern "C" {
#endif

//初始化所有 UI 业务模块并绑定事件回调
void ui_events_init(void);

#ifdef __cplusplus
}
#endif

#endif /* UI_EVENTS_INIT_H */
