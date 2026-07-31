#ifndef EEZ_LVGL_UI_SCREENS_H
#define EEZ_LVGL_UI_SCREENS_H

#include <lvgl/lvgl.h>

#ifdef __cplusplus
extern "C" {
#endif

// Screens

enum ScreensEnum {
    _SCREEN_ID_FIRST = 1,
    SCREEN_ID_MAIN = 1,
    _SCREEN_ID_LAST = 1
};

typedef struct _objects_t {
    lv_obj_t *main;
    lv_obj_t *carmodel;
    lv_obj_t *l3_2;
    lv_obj_t *l3_3;
    lv_obj_t *l3_1;
    lv_obj_t *l3_4;
    lv_obj_t *tem_roller;
    lv_obj_t *meter;
    lv_obj_t *airconditionr_call_btn;
    lv_obj_t *flash_btn;
    lv_obj_t *lock_btn;
    lv_obj_t *trunk_btn;
    lv_obj_t *obj0;
    lv_obj_t *funk_btn;
    lv_obj_t *frunk_label;
    lv_obj_t *flash1;
    lv_obj_t *flash3;
    lv_obj_t *flash4;
    lv_obj_t *flash2;
    lv_obj_t *player_widget_panel;
    lv_obj_t *song_name;
    lv_obj_t *song_panel;
    lv_obj_t *backword_btn;
    lv_obj_t *pause_btn;
    lv_obj_t *forward_btn;
    lv_obj_t *full_btn;
    lv_obj_t *time_wealth_panel;
    lv_obj_t *time_label;
    lv_obj_t *calander_btn;
    lv_obj_t *year_label;
    lv_obj_t *wealth_btn;
    lv_obj_t *ai_chatbox;
    lv_obj_t *ai_answer;
    lv_obj_t *ai_searchbox;
    lv_obj_t *ai_send_btn;
    lv_obj_t *ai_chatbox_shutbtn;
    lv_obj_t *ai_userrequest;
    lv_obj_t *wealth_panel;
    lv_obj_t *l3;
    lv_obj_t *day2label;
    lv_obj_t *l2;
    lv_obj_t *l1;
    lv_obj_t *day3label;
    lv_obj_t *day1label;
    lv_obj_t *wuhan_label;
    lv_obj_t *day3icon;
    lv_obj_t *day2icon;
    lv_obj_t *day1icon;
    lv_obj_t *calander;
    lv_obj_t *player_main_panel;
    lv_obj_t *songer;
    lv_obj_t *progress_bar;
    lv_obj_t *song_panel_main;
    lv_obj_t *backword_main_btn;
    lv_obj_t *forward_main_btn;
    lv_obj_t *pause_main_btn;
    lv_obj_t *song_tittle;
    lv_obj_t *full_btn_1;
    lv_obj_t *songlist_btn;
    lv_obj_t *cloude_icon;
    lv_obj_t *songlist;
    lv_obj_t *sound_btn;
    lv_obj_t *sound_bar;
    lv_obj_t *brightness_btn;
    lv_obj_t *brightness_bar;
    lv_obj_t *ai_chatkeyboard;
    lv_obj_t *l3_5;
} objects_t;

extern objects_t objects;

typedef struct {
    lv_meter_scale_t *scale;
    lv_meter_indicator_t *indicator;
} screen_main_state_t;

extern screen_main_state_t screen_main_state;

void create_screen_main();
void tick_screen_main();

void tick_screen_by_id(enum ScreensEnum screenId);
void tick_screen(int screen_index);

void create_screens();

#ifdef __cplusplus
}
#endif

#endif /*EEZ_LVGL_UI_SCREENS_H*/