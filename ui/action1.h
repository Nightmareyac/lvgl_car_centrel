#ifndef USER_ACTIONS_H
#define USER_ACTIONS_H

#include <lvgl/lvgl.h>

// 函数声明
void expand_main_panel(lv_event_t * e);
void collapse_main_panel(lv_event_t * e);
void play_pause_cb(lv_event_t * e);
void next_song_cb(lv_event_t * e);
void prev_song_cb(lv_event_t * e);
void player_events_init(void);
void bemfa_ui_update_cb(lv_timer_t * timer);

#endif /* USER_ACTIONS_H */