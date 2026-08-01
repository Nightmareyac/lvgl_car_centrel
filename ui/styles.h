#ifndef EEZ_LVGL_UI_STYLES_H
#define EEZ_LVGL_UI_STYLES_H

#include <lvgl/lvgl.h>

#ifdef __cplusplus
extern "C" {
#endif

// Style: click_style
lv_style_t *get_style_click_style_MAIN_DEFAULT();
lv_style_t *get_style_click_style_MAIN_PRESSED();
lv_style_t *get_style_click_style_MAIN_FOCUSED();
lv_style_t *get_style_click_style_MAIN_CHECKED();
lv_style_t *get_style_click_style_MAIN_CHECKED_PRESSED();
void add_style_click_style(lv_obj_t *obj);
void remove_style_click_style(lv_obj_t *obj);

// Style: checkstyle
lv_style_t *get_style_checkstyle_MAIN_DEFAULT();
lv_style_t *get_style_checkstyle_MAIN_CHECKED();
lv_style_t *get_style_checkstyle_MAIN_PRESSED();
lv_style_t *get_style_checkstyle_MAIN_FOCUSED();
lv_style_t *get_style_checkstyle_MAIN_CHECKED_PRESSED();
void add_style_checkstyle(lv_obj_t *obj);
void remove_style_checkstyle(lv_obj_t *obj);

// Style: darkslider
lv_style_t *get_style_darkslider_MAIN_CHECKED();
lv_style_t *get_style_darkslider_MAIN_DEFAULT();
lv_style_t *get_style_darkslider_INDICATOR_DEFAULT();
lv_style_t *get_style_darkslider_KNOB_DEFAULT();
void add_style_darkslider(lv_obj_t *obj);
void remove_style_darkslider(lv_obj_t *obj);

// Style: FRUNK
lv_style_t *get_style_frunk_MAIN_DEFAULT();
void add_style_frunk(lv_obj_t *obj);
void remove_style_frunk(lv_obj_t *obj);

// Style: greyfont
lv_style_t *get_style_greyfont_MAIN_DEFAULT();
void add_style_greyfont(lv_obj_t *obj);
void remove_style_greyfont(lv_obj_t *obj);

#ifdef __cplusplus
}
#endif

#endif /*EEZ_LVGL_UI_STYLES_H*/