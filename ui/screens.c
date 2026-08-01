#include <string.h>

#include "screens.h"
#include "images.h"
#include "fonts.h"
#include "actions.h"
#include "vars.h"
#include "styles.h"
#include "ui.h"

#include <string.h>

objects_t objects;

screen_main_state_t screen_main_state;

//
// Event handlers
//

lv_obj_t *tick_value_change_obj;

//
// Screens
//

void create_screen_main() {
    screen_main_state_t *state = &screen_main_state;
    (void)state;
    lv_obj_t *obj = lv_obj_create(0);
    objects.main = obj;
    lv_obj_set_pos(obj, 0, 0);
    lv_obj_set_size(obj, 1024, 600);
    lv_obj_set_style_bg_img_opa(obj, 50, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(obj, lv_color_hex(0xececec), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(obj, LV_GRAD_DIR_VER, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_color(obj, lv_color_hex(0xd8d8d8), LV_PART_MAIN | LV_STATE_DEFAULT);
    {
        lv_obj_t *parent_obj = obj;
        {
            // carmodel
            lv_obj_t *obj = lv_obj_create(parent_obj);
            objects.carmodel = obj;
            lv_obj_set_pos(obj, 258, 184);
            lv_obj_set_size(obj, 527, 266);
            lv_obj_set_style_pad_left(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_pad_top(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_pad_right(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_pad_bottom(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_bg_opa(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_border_width(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_radius(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
            add_style_frunk(obj);
        }
        {
            // l3_2
            lv_obj_t *obj = lv_line_create(parent_obj);
            objects.l3_2 = obj;
            lv_obj_set_pos(obj, 512, 129);
            lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
            static lv_point_t line_points[] = {
                { 10, 20 },
                { 10, 100 }
            };
            lv_line_set_points(obj, line_points, 2);
            lv_line_set_y_invert(obj, true);
            lv_obj_set_style_line_width(obj, 2, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_line_color(obj, lv_color_hex(0x787878), LV_PART_MAIN | LV_STATE_DEFAULT);
        }
        {
            // l3_3
            lv_obj_t *obj = lv_line_create(parent_obj);
            objects.l3_3 = obj;
            lv_obj_set_pos(obj, 739, 184);
            lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
            static lv_point_t line_points[] = {
                { 10, 20 },
                { 10, 100 }
            };
            lv_line_set_points(obj, line_points, 2);
            lv_line_set_y_invert(obj, true);
            lv_obj_set_style_line_width(obj, 2, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_line_color(obj, lv_color_hex(0x787878), LV_PART_MAIN | LV_STATE_DEFAULT);
        }
        {
            // l3_1
            lv_obj_t *obj = lv_line_create(parent_obj);
            objects.l3_1 = obj;
            lv_obj_set_pos(obj, 319, 197);
            lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
            static lv_point_t line_points[] = {
                { 20, 20 },
                { 20, 100 }
            };
            lv_line_set_points(obj, line_points, 2);
            lv_line_set_y_invert(obj, true);
            lv_obj_set_style_line_width(obj, 2, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_line_color(obj, lv_color_hex(0x787878), LV_PART_MAIN | LV_STATE_DEFAULT);
        }
        {
            // l3_4
            lv_obj_t *obj = lv_line_create(parent_obj);
            objects.l3_4 = obj;
            lv_obj_set_pos(obj, 762, 281);
            lv_obj_set_size(obj, 55, 17);
            static lv_point_t line_points[] = {
                { 10, 10 },
                { 50, 10 }
            };
            lv_line_set_points(obj, line_points, 2);
            lv_line_set_y_invert(obj, true);
            lv_obj_set_style_line_width(obj, 2, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_line_color(obj, lv_color_hex(0x787878), LV_PART_MAIN | LV_STATE_DEFAULT);
        }
        {
            // l3_5
            lv_obj_t *obj = lv_line_create(parent_obj);
            objects.l3_5 = obj;
            lv_obj_set_pos(obj, 212, 362);
            lv_obj_set_size(obj, 55, 17);
            static lv_point_t line_points[] = {
                { 10, 10 },
                { 50, 10 }
            };
            lv_line_set_points(obj, line_points, 2);
            lv_line_set_y_invert(obj, true);
            lv_obj_set_style_line_width(obj, 2, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_line_color(obj, lv_color_hex(0x787878), LV_PART_MAIN | LV_STATE_DEFAULT);
        }
        {
            // tem_roller
            lv_obj_t *obj = lv_roller_create(parent_obj);
            objects.tem_roller = obj;
            lv_obj_set_pos(obj, 126, 352);
            lv_obj_set_size(obj, 56, 37);
            lv_roller_set_options(obj, "16\n17\n18\n19\n20\n21\n22\n23\n24\n25\n26\n27\n28\n29\n30\n31\n21", LV_ROLLER_MODE_NORMAL);
            lv_obj_add_flag(obj, LV_OBJ_FLAG_HIDDEN);
            lv_obj_set_style_bg_color(obj, lv_color_hex(0xb6b6b6), LV_PART_SELECTED | LV_STATE_DEFAULT);
            lv_obj_set_style_bg_color(obj, lv_color_hex(0xffffff), LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_bg_opa(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
        }
        {
            lv_obj_t *obj = lv_img_create(parent_obj);
            lv_obj_set_pos(obj, 26, 22);
            lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
            lv_img_set_src(obj, &img_telsa);
        }
        {
            // Meter
            lv_obj_t *obj = lv_meter_create(parent_obj);
            objects.meter = obj;
            lv_obj_set_pos(obj, 137, 38);
            lv_obj_set_size(obj, 170, 170);
            {
                lv_meter_scale_t *scale = lv_meter_add_scale(obj);
                state->scale = scale;
                lv_meter_set_scale_ticks(obj, scale, 41, 1, 5, lv_color_hex(0xa0a0a0));
                lv_meter_set_scale_major_ticks(obj, scale, 8, 3, 10, lv_color_hex(0x000000), 10);
                lv_meter_set_scale_range(obj, scale, 0, 100, 300, 120);
                {
                    lv_meter_indicator_t *indicator = lv_meter_add_needle_line(obj, scale, 3, lv_color_hex(0x5f5f5f), -28);
                    state->indicator = indicator;
                    lv_meter_set_indicator_value(obj, indicator, 50);
                }
            }
            lv_obj_set_style_bg_opa(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_bg_color(obj, lv_color_hex(0xffffff), LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_border_color(obj, lv_color_hex(0xe0e0e0), LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_border_width(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
        }
        {
            // airconditionr_call_btn
            lv_obj_t *obj = lv_btn_create(parent_obj);
            objects.airconditionr_call_btn = obj;
            lv_obj_set_pos(obj, 184, 354);
            lv_obj_set_size(obj, 35, 32);
            lv_obj_add_flag(obj, LV_OBJ_FLAG_CHECKABLE);
            lv_obj_clear_flag(obj, LV_OBJ_FLAG_CLICK_FOCUSABLE);
            lv_obj_add_state(obj, LV_STATE_CHECKED);
            add_style_click_style(obj);
            lv_obj_set_style_bg_img_src(obj, &img_air, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_bg_img_recolor(obj, lv_color_hex(0x000000), LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_bg_img_opa(obj, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_bg_img_recolor_opa(obj, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_bg_img_recolor_opa(obj, 255, LV_PART_MAIN | LV_STATE_CHECKED);
            lv_obj_set_style_bg_img_src(obj, &img_air, LV_PART_MAIN | LV_STATE_CHECKED);
            lv_obj_set_style_bg_opa(obj, 255, LV_PART_MAIN | LV_STATE_CHECKED);
            lv_obj_set_style_bg_color(obj, lv_color_hex(0x303030), LV_PART_MAIN | LV_STATE_CHECKED);
            lv_obj_set_style_bg_img_recolor(obj, lv_color_hex(0xffffff), LV_PART_MAIN | LV_STATE_CHECKED);
            lv_obj_set_style_radius(obj, 10, LV_PART_MAIN | LV_STATE_CHECKED);
            lv_obj_set_style_bg_opa(obj, 0, LV_PART_MAIN | LV_STATE_PRESSED);
        }
        {
            // flash_btn
            lv_obj_t *obj = lv_btn_create(parent_obj);
            objects.flash_btn = obj;
            lv_obj_set_pos(obj, 815, 275);
            lv_obj_set_size(obj, 25, 25);
            lv_obj_clear_flag(obj, LV_OBJ_FLAG_CLICKABLE|LV_OBJ_FLAG_CLICK_FOCUSABLE);
            add_style_checkstyle(obj);
            lv_obj_set_style_bg_img_src(obj, &img_flash, LV_PART_MAIN | LV_STATE_DEFAULT);
        }
        {
            // lock_btn
            lv_obj_t *obj = lv_btn_create(parent_obj);
            objects.lock_btn = obj;
            lv_obj_set_pos(obj, 512, 103);
            lv_obj_set_size(obj, 25, 25);
            lv_obj_add_flag(obj, LV_OBJ_FLAG_CHECKABLE);
            lv_obj_clear_flag(obj, LV_OBJ_FLAG_CLICK_FOCUSABLE);
            lv_obj_add_state(obj, LV_STATE_CHECKED);
            add_style_checkstyle(obj);
        }
        {
            // trunk_btn
            lv_obj_t *obj = lv_btn_create(parent_obj);
            objects.trunk_btn = obj;
            lv_obj_set_pos(obj, 757, 169);
            lv_obj_set_size(obj, 56, 46);
            add_style_click_style(obj);
            lv_obj_set_style_bg_color(obj, lv_color_hex(0x3d3d3d), LV_PART_MAIN | LV_STATE_PRESSED);
            lv_obj_set_style_radius(obj, 10, LV_PART_MAIN | LV_STATE_PRESSED);
            lv_obj_set_style_bg_opa(obj, 0, LV_PART_MAIN | LV_STATE_FOCUSED);
            lv_obj_set_style_bg_color(obj, lv_color_hex(0xffffff), LV_PART_MAIN | LV_STATE_FOCUSED);
            {
                lv_obj_t *parent_obj = obj;
                {
                    lv_obj_t *obj = lv_label_create(parent_obj);
                    objects.obj0 = obj;
                    lv_obj_set_pos(obj, 1, 1);
                    lv_obj_set_size(obj, 46, 38);
                    lv_obj_add_flag(obj, LV_OBJ_FLAG_CHECKABLE);
                    lv_obj_clear_flag(obj, LV_OBJ_FLAG_CLICK_FOCUSABLE);
                    lv_obj_set_style_align(obj, LV_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_text_color(obj, lv_color_hex(0x545454), LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_label_set_text_static(obj, "Open\nTrunk");
                }
            }
        }
        {
            // Funk_btn
            lv_obj_t *obj = lv_btn_create(parent_obj);
            objects.funk_btn = obj;
            lv_obj_set_pos(obj, 349, 184);
            lv_obj_set_size(obj, 59, 48);
            lv_obj_add_flag(obj, LV_OBJ_FLAG_CHECKABLE);
            lv_obj_clear_flag(obj, LV_OBJ_FLAG_CLICK_FOCUSABLE);
            add_style_click_style(obj);
            {
                lv_obj_t *parent_obj = obj;
                {
                    // frunk_label
                    lv_obj_t *obj = lv_label_create(parent_obj);
                    objects.frunk_label = obj;
                    lv_obj_set_pos(obj, 1, 2);
                    lv_obj_set_size(obj, 42, 37);
                    lv_obj_clear_flag(obj, LV_OBJ_FLAG_CLICK_FOCUSABLE);
                    lv_obj_set_style_align(obj, LV_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_text_color(obj, lv_color_hex(0x464646), LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_text_opa(obj, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_text_color(obj, lv_color_hex(0xffffff), LV_PART_MAIN | LV_STATE_CHECKED | LV_STATE_PRESSED);
                    lv_obj_set_style_text_opa(obj, 255, LV_PART_MAIN | LV_STATE_CHECKED | LV_STATE_PRESSED);
                    lv_label_set_text_static(obj, "Open\nFunk");
                }
            }
        }
        {
            // music_all_panel
            lv_obj_t *obj = lv_obj_create(parent_obj);
            objects.music_all_panel = obj;
            lv_obj_set_pos(obj, 10, 0);
            lv_obj_set_size(obj, 330, 600);
            lv_obj_set_style_pad_left(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_pad_top(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_pad_right(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_pad_bottom(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_bg_opa(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_border_width(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_radius(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_add_flag(obj, LV_OBJ_FLAG_HIDDEN);
            lv_obj_clear_flag(obj, LV_OBJ_FLAG_CLICKABLE);
            {
                lv_obj_t *parent_obj = obj;
                {
                    // flash1
                    lv_obj_t *obj = lv_btn_create(parent_obj);
                    objects.flash1 = obj;
                    lv_obj_set_pos(obj, 38, 130);
                    lv_obj_set_size(obj, 40, 40);
                    lv_obj_add_flag(obj, LV_OBJ_FLAG_CHECKABLE);
                    lv_obj_clear_flag(obj, LV_OBJ_FLAG_CLICK_FOCUSABLE);
                    add_style_click_style(obj);
                    lv_obj_set_style_bg_img_src(obj, &img_flash3, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_bg_img_recolor(obj, lv_color_hex(0xa2a3a5), LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_bg_img_recolor_opa(obj, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_bg_img_recolor_opa(obj, 0, LV_PART_MAIN | LV_STATE_CHECKED);
                    lv_obj_set_style_bg_opa(obj, 0, LV_PART_MAIN | LV_STATE_CHECKED);
                    lv_obj_set_style_bg_opa(obj, 0, LV_PART_MAIN | LV_STATE_PRESSED);
                }
                {
                    // flash2
                    lv_obj_t *obj = lv_btn_create(parent_obj);
                    objects.flash2 = obj;
                    lv_obj_set_pos(obj, 38, 209);
                    lv_obj_set_size(obj, 40, 40);
                    lv_obj_add_flag(obj, LV_OBJ_FLAG_CHECKABLE);
                    lv_obj_clear_flag(obj, LV_OBJ_FLAG_CLICK_FOCUSABLE);
                    add_style_click_style(obj);
                    lv_obj_set_style_bg_img_src(obj, &img_flash2, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_bg_opa(obj, 0, LV_PART_MAIN | LV_STATE_CHECKED);
                    lv_obj_set_style_bg_img_recolor(obj, lv_color_hex(0x43d16d), LV_PART_MAIN | LV_STATE_CHECKED);
                    lv_obj_set_style_bg_img_recolor_opa(obj, 255, LV_PART_MAIN | LV_STATE_CHECKED);
                    lv_obj_set_style_bg_opa(obj, 0, LV_PART_MAIN | LV_STATE_PRESSED);
                }
                {
                    // flash3
                    lv_obj_t *obj = lv_btn_create(parent_obj);
                    objects.flash3 = obj;
                    lv_obj_set_pos(obj, 38, 289);
                    lv_obj_set_size(obj, 40, 40);
                    lv_obj_add_flag(obj, LV_OBJ_FLAG_CHECKABLE);
                    lv_obj_clear_flag(obj, LV_OBJ_FLAG_CLICK_FOCUSABLE);
                    add_style_click_style(obj);
                    lv_obj_set_style_bg_img_src(obj, &img_flash1, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_bg_img_recolor(obj, lv_color_hex(0x97989a), LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_bg_img_opa(obj, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_bg_img_recolor_opa(obj, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_bg_opa(obj, 0, LV_PART_MAIN | LV_STATE_CHECKED);
                    lv_obj_set_style_bg_img_recolor_opa(obj, 0, LV_PART_MAIN | LV_STATE_CHECKED);
                    lv_obj_set_style_bg_opa(obj, 0, LV_PART_MAIN | LV_STATE_PRESSED);
                }
                {
                    // flash4
                    lv_obj_t *obj = lv_btn_create(parent_obj);
                    objects.flash4 = obj;
                    lv_obj_set_pos(obj, 38, 379);
                    lv_obj_set_size(obj, 40, 42);
                    lv_obj_add_flag(obj, LV_OBJ_FLAG_CHECKABLE);
                    lv_obj_clear_flag(obj, LV_OBJ_FLAG_CLICK_FOCUSABLE);
                    add_style_click_style(obj);
                    lv_obj_set_style_bg_img_src(obj, &img_safebelt, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_bg_img_opa(obj, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_bg_img_recolor(obj, lv_color_hex(0x97989a), LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_bg_img_recolor_opa(obj, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_bg_opa(obj, 0, LV_PART_MAIN | LV_STATE_CHECKED);
                    lv_obj_set_style_bg_img_recolor_opa(obj, 0, LV_PART_MAIN | LV_STATE_CHECKED);
                    lv_obj_set_style_bg_img_recolor(obj, lv_color_hex(0xc00000), LV_PART_MAIN | LV_STATE_CHECKED);
                    lv_obj_set_style_bg_img_opa(obj, 255, LV_PART_MAIN | LV_STATE_CHECKED);
                    lv_obj_set_style_bg_opa(obj, 0, LV_PART_MAIN | LV_STATE_PRESSED);
                }
                {
                    // Player_Widget_panel
                    lv_obj_t *obj = lv_obj_create(parent_obj);
                    objects.player_widget_panel = obj;
                    lv_obj_set_pos(obj, 10, 464);
                    lv_obj_set_size(obj, 301, 118);
                    lv_obj_clear_flag(obj, LV_OBJ_FLAG_SCROLLABLE);
                    lv_obj_set_style_radius(obj, 20, LV_PART_MAIN | LV_STATE_DEFAULT);
                    {
                        lv_obj_t *parent_obj = obj;
                        {
                            // song name
                            lv_obj_t *obj = lv_label_create(parent_obj);
                            objects.song_name = obj;
                            lv_obj_set_pos(obj, 95, -6);
                            lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                            lv_obj_set_style_text_font(obj, &lv_font_montserrat_22, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_label_set_text_static(obj, "Love song");
                        }
                        {
                            // song_panel
                            lv_obj_t *obj = lv_obj_create(parent_obj);
                            objects.song_panel = obj;
                            lv_obj_set_pos(obj, -13, -13);
                            lv_obj_set_size(obj, 100, 100);
                            lv_obj_clear_flag(obj, LV_OBJ_FLAG_CLICKABLE|LV_OBJ_FLAG_CLICK_FOCUSABLE|LV_OBJ_FLAG_SCROLLABLE);
                            lv_obj_set_style_radius(obj, 50, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_border_color(obj, lv_color_hex(0x333131), LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_border_width(obj, 15, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_clip_corner(obj, true, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_bg_img_opa(obj, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_bg_opa(obj, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_bg_img_tiled(obj, false, LV_PART_MAIN | LV_STATE_DEFAULT);
                        }
                        {
                            // backword_btn
                            lv_obj_t *obj = lv_btn_create(parent_obj);
                            objects.backword_btn = obj;
                            lv_obj_set_pos(obj, 94, 38);
                            lv_obj_set_size(obj, 42, 42);
                            add_style_click_style(obj);
                            lv_obj_set_style_radius(obj, 25, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_bg_img_src(obj, &img_backwardimg, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_bg_img_recolor(obj, lv_color_hex(0x545454), LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_bg_img_recolor_opa(obj, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_bg_img_opa(obj, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
                        }
                        {
                            // pause_btn
                            lv_obj_t *obj = lv_btn_create(parent_obj);
                            objects.pause_btn = obj;
                            lv_obj_set_pos(obj, 151, 38);
                            lv_obj_set_size(obj, 42, 42);
                            lv_obj_add_flag(obj, LV_OBJ_FLAG_CHECKABLE);
                            lv_obj_clear_flag(obj, LV_OBJ_FLAG_CLICK_FOCUSABLE);
                            add_style_click_style(obj);
                            lv_obj_set_style_bg_img_src(obj, &img_pauseimg, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_bg_img_opa(obj, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_bg_img_recolor(obj, lv_color_hex(0x545454), LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_bg_img_recolor_opa(obj, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_bg_opa(obj, 0, LV_PART_MAIN | LV_STATE_CHECKED);
                            lv_obj_set_style_bg_img_src(obj, &img_playimg, LV_PART_MAIN | LV_STATE_CHECKED);
                            lv_obj_set_style_bg_opa(obj, 0, LV_PART_MAIN | LV_STATE_PRESSED);
                            lv_obj_set_style_bg_color(obj, lv_color_hex(0x444444), LV_PART_MAIN | LV_STATE_PRESSED);
                        }
                        {
                            // forward_btn
                            lv_obj_t *obj = lv_btn_create(parent_obj);
                            objects.forward_btn = obj;
                            lv_obj_set_pos(obj, 207, 38);
                            lv_obj_set_size(obj, 42, 42);
                            add_style_click_style(obj);
                            lv_obj_set_style_radius(obj, 25, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_bg_img_src(obj, &img_forwardimg, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_bg_img_opa(obj, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_bg_img_recolor(obj, lv_color_hex(0x545454), LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_bg_img_recolor_opa(obj, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
                        }
                        {
                            // full_btn
                            lv_obj_t *obj = lv_btn_create(parent_obj);
                            objects.full_btn = obj;
                            lv_obj_set_pos(obj, 228, -13);
                            lv_obj_set_size(obj, 40, 38);
                            add_style_click_style(obj);
                            lv_obj_set_style_radius(obj, 25, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_bg_img_src(obj, &img_fullimg, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_bg_img_opa(obj, 180, LV_PART_MAIN | LV_STATE_DEFAULT);
                        }
                    }
                }
                {
                    // Player_Main_panel
                    lv_obj_t *obj = lv_obj_create(parent_obj);
                    objects.player_main_panel = obj;
                    lv_obj_set_pos(obj, 11, 22);
                    lv_obj_set_size(obj, 308, 565);
                    lv_obj_clear_flag(obj, LV_OBJ_FLAG_SCROLLABLE);
                    lv_obj_set_style_radius(obj, 30, LV_PART_MAIN | LV_STATE_DEFAULT);
                    {
                        lv_obj_t *parent_obj = obj;
                        {
                            // songer
                            lv_obj_t *obj = lv_label_create(parent_obj);
                            objects.songer = obj;
                            lv_obj_set_pos(obj, 3, 351);
                            lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                            lv_obj_set_style_text_font(obj, &lv_font_montserrat_22, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_label_set_text_static(obj, "JAY");
                        }
                        {
                            // progress_bar
                            lv_obj_t *obj = lv_bar_create(parent_obj);
                            objects.progress_bar = obj;
                            lv_obj_set_pos(obj, 3, 416);
                            lv_obj_set_size(obj, 257, 16);
                            lv_bar_set_value(obj, 25, LV_ANIM_OFF);
                            lv_obj_set_style_bg_color(obj, lv_color_hex(0x757575), LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_bg_color(obj, lv_color_hex(0x646464), LV_PART_INDICATOR | LV_STATE_DEFAULT);
                        }
                        {
                            // song_panel_main
                            lv_obj_t *obj = lv_obj_create(parent_obj);
                            objects.song_panel_main = obj;
                            lv_obj_set_pos(obj, 15, 47);
                            lv_obj_set_size(obj, 230, 230);
                            lv_obj_clear_flag(obj, LV_OBJ_FLAG_CLICKABLE|LV_OBJ_FLAG_CLICK_FOCUSABLE|LV_OBJ_FLAG_SCROLLABLE);
                            lv_obj_set_style_radius(obj, 120, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_border_color(obj, lv_color_hex(0x333131), LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_border_width(obj, 15, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_clip_corner(obj, true, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_bg_img_opa(obj, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_bg_img_tiled(obj, false, LV_PART_MAIN | LV_STATE_DEFAULT);
                        }
                        {
                            // backword_main_btn
                            lv_obj_t *obj = lv_btn_create(parent_obj);
                            objects.backword_main_btn = obj;
                            lv_obj_set_pos(obj, 5, 463);
                            lv_obj_set_size(obj, 42, 42);
                            lv_obj_clear_flag(obj, LV_OBJ_FLAG_PRESS_LOCK|LV_OBJ_FLAG_SCROLL_MOMENTUM|LV_OBJ_FLAG_SCROLL_ON_FOCUS);
                            add_style_click_style(obj);
                            lv_obj_set_style_radius(obj, 25, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_bg_img_src(obj, &img_backwardimg, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_bg_opa(obj, 0, LV_PART_MAIN | LV_STATE_FOCUSED);
                            lv_obj_set_style_bg_color(obj, lv_color_hex(0x000000), LV_PART_MAIN | LV_STATE_FOCUSED);
                        }
                        {
                            // forward_main_btn
                            lv_obj_t *obj = lv_btn_create(parent_obj);
                            objects.forward_main_btn = obj;
                            lv_obj_set_pos(obj, 156, 463);
                            lv_obj_set_size(obj, 42, 42);
                            add_style_click_style(obj);
                            lv_obj_set_style_radius(obj, 25, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_bg_img_src(obj, &img_forwardimg, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_bg_opa(obj, 0, LV_PART_MAIN | LV_STATE_FOCUSED);
                            lv_obj_set_style_bg_color(obj, lv_color_hex(0x000000), LV_PART_MAIN | LV_STATE_FOCUSED);
                        }
                        {
                            // pause_main_btn
                            lv_obj_t *obj = lv_btn_create(parent_obj);
                            objects.pause_main_btn = obj;
                            lv_obj_set_pos(obj, 88, 463);
                            lv_obj_set_size(obj, 42, 42);
                            lv_obj_add_flag(obj, LV_OBJ_FLAG_CHECKABLE);
                            lv_obj_clear_flag(obj, LV_OBJ_FLAG_CLICK_FOCUSABLE);
                            add_style_click_style(obj);
                            lv_obj_set_style_bg_img_src(obj, &img_pauseimg, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_bg_img_src(obj, &img_playimg, LV_PART_MAIN | LV_STATE_CHECKED);
                            lv_obj_set_style_bg_opa(obj, 0, LV_PART_MAIN | LV_STATE_CHECKED);
                            lv_obj_set_style_bg_opa(obj, 0, LV_PART_MAIN | LV_STATE_PRESSED);
                            lv_obj_set_style_bg_opa(obj, 0, LV_PART_MAIN | LV_STATE_FOCUSED);
                        }
                        {
                            // song_tittle
                            lv_obj_t *obj = lv_label_create(parent_obj);
                            objects.song_tittle = obj;
                            lv_obj_set_pos(obj, 3, 301);
                            lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                            lv_obj_set_style_text_font(obj, &lv_font_montserrat_36, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_label_set_text_static(obj, "Love song");
                        }
                        {
                            // full_btn_1
                            lv_obj_t *obj = lv_btn_create(parent_obj);
                            objects.full_btn_1 = obj;
                            lv_obj_set_pos(obj, 237, -8);
                            lv_obj_set_size(obj, 31, 34);
                            add_style_click_style(obj);
                            lv_obj_set_style_radius(obj, 25, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_bg_img_src(obj, &img_fullexit, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_bg_img_opa(obj, 180, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_bg_opa(obj, 0, LV_PART_MAIN | LV_STATE_FOCUSED);
                            lv_obj_set_style_bg_color(obj, lv_color_hex(0x000000), LV_PART_MAIN | LV_STATE_FOCUSED);
                        }
                        {
                            // songlist_btn
                            lv_obj_t *obj = lv_btn_create(parent_obj);
                            objects.songlist_btn = obj;
                            lv_obj_set_pos(obj, 218, 463);
                            lv_obj_set_size(obj, 42, 42);
                            lv_obj_add_flag(obj, LV_OBJ_FLAG_CHECKABLE);
                            lv_obj_clear_flag(obj, LV_OBJ_FLAG_CLICK_FOCUSABLE);
                            add_style_click_style(obj);
                            lv_obj_set_style_radius(obj, 10, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_bg_img_src(obj, &img_playelist, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_bg_img_recolor(obj, lv_color_hex(0x333333), LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_bg_img_recolor_opa(obj, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_bg_color(obj, lv_color_hex(0x5c5c5c), LV_PART_MAIN | LV_STATE_CHECKED);
                            lv_obj_set_style_radius(obj, 10, LV_PART_MAIN | LV_STATE_CHECKED);
                            lv_obj_set_style_bg_opa(obj, 0, LV_PART_MAIN | LV_STATE_FOCUSED);
                        }
                        {
                            // cloudeIcon
                            lv_obj_t *obj = lv_img_create(parent_obj);
                            objects.cloude_icon = obj;
                            lv_obj_set_pos(obj, -5, -8);
                            lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                            lv_img_set_src(obj, &img_cloudmusice_icon);
                        }
                    }
                }
            }
        }
        {
            // time_wealth_panel
            lv_obj_t *obj = lv_obj_create(parent_obj);
            objects.time_wealth_panel = obj;
            lv_obj_set_pos(obj, 804, 8);
            lv_obj_set_size(obj, 210, 108);
            lv_obj_set_style_pad_left(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_pad_top(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_pad_right(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_pad_bottom(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_border_width(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_clear_flag(obj, LV_OBJ_FLAG_SCROLLABLE);
            lv_obj_set_style_bg_color(obj, lv_color_hex(0xb7b7b7), LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_bg_opa(obj, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_radius(obj, 30, LV_PART_MAIN | LV_STATE_DEFAULT);
            {
                lv_obj_t *parent_obj = obj;
                {
                    // time_label
                    lv_obj_t *obj = lv_label_create(parent_obj);
                    objects.time_label = obj;
                    lv_obj_set_pos(obj, 39, -16);
                    lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                    lv_obj_set_style_align(obj, LV_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_text_font(obj, &lv_font_montserrat_44, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_text_color(obj, lv_color_hex(0x646464), LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_label_set_text_static(obj, "12:33");
                }
                {
                    // calanderBtn
                    lv_obj_t *obj = lv_btn_create(parent_obj);
                    objects.calander_btn = obj;
                    lv_obj_set_pos(obj, 9, 57);
                    lv_obj_set_size(obj, 198, 38);
                    lv_obj_add_flag(obj, LV_OBJ_FLAG_CHECKABLE);
                    lv_obj_clear_flag(obj, LV_OBJ_FLAG_CLICK_FOCUSABLE);
                    add_style_click_style(obj);
                    lv_obj_set_style_bg_opa(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_text_color(obj, lv_color_hex(0x303030), LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_radius(obj, 15, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_bg_color(obj, lv_color_hex(0x7e7e7e), LV_PART_MAIN | LV_STATE_CHECKED);
                    lv_obj_set_style_bg_opa(obj, 255, LV_PART_MAIN | LV_STATE_CHECKED);
                    {
                        lv_obj_t *parent_obj = obj;
                        {
                            // year_label
                            lv_obj_t *obj = lv_label_create(parent_obj);
                            objects.year_label = obj;
                            lv_obj_set_pos(obj, 0, -4);
                            lv_obj_set_size(obj, 196, 33);
                            lv_obj_set_style_align(obj, LV_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_text_font(obj, &lv_font_montserrat_38, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_text_color(obj, lv_color_hex(0x646464), LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_label_set_text_static(obj, "2026.03.15");
                        }
                    }
                }
                {
                    // wealth_btn
                    lv_obj_t *obj = lv_btn_create(parent_obj);
                    objects.wealth_btn = obj;
                    lv_obj_set_pos(obj, 12, 5);
                    lv_obj_set_size(obj, 75, 57);
                    lv_obj_add_flag(obj, LV_OBJ_FLAG_CHECKABLE);
                    lv_obj_clear_flag(obj, LV_OBJ_FLAG_CLICK_FOCUSABLE);
                    add_style_click_style(obj);
                    lv_obj_set_style_bg_img_src(obj, &img_wealth, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_bg_img_opa(obj, 200, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_bg_img_recolor(obj, lv_color_hex(0xffffff), LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_bg_opa(obj, 255, LV_PART_MAIN | LV_STATE_CHECKED);
                    lv_obj_set_style_bg_color(obj, lv_color_hex(0x7e7e7e), LV_PART_MAIN | LV_STATE_CHECKED);
                    lv_obj_set_style_bg_img_src(obj, &img_wealth, LV_PART_MAIN | LV_STATE_CHECKED);
                    lv_obj_set_style_bg_img_recolor(obj, lv_color_hex(0xffffff), LV_PART_MAIN | LV_STATE_CHECKED);
                    lv_obj_set_style_bg_img_recolor_opa(obj, 255, LV_PART_MAIN | LV_STATE_CHECKED);
                }
            }
        }
        {
            // ai_chatbox
            lv_obj_t *obj = lv_obj_create(parent_obj);
            objects.ai_chatbox = obj;
            lv_obj_set_pos(obj, 330, 211);
            lv_obj_set_size(obj, 470, 380);
            lv_obj_add_flag(obj, LV_OBJ_FLAG_HIDDEN);
            lv_obj_set_style_radius(obj, 50, LV_PART_MAIN | LV_STATE_FOCUSED);
            {
                lv_obj_t *parent_obj = obj;
                {
                    // ai_answer
                    lv_obj_t *obj = lv_label_create(parent_obj);
                    objects.ai_answer = obj;
                    lv_obj_set_pos(obj, 8, 2);
                    lv_obj_set_size(obj, 426, 273);
                    lv_obj_set_style_text_font(obj, &lv_font_montserrat_20, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_label_set_text_static(obj, "Text");
                }
            }
        }
        {
            // ai_searchbox
            lv_obj_t *obj = lv_obj_create(parent_obj);
            objects.ai_searchbox = obj;
            lv_obj_set_pos(obj, 349, 515);
            lv_obj_set_size(obj, 436, 66);
            lv_obj_clear_flag(obj, LV_OBJ_FLAG_CLICKABLE|LV_OBJ_FLAG_CLICK_FOCUSABLE|LV_OBJ_FLAG_SCROLLABLE);
            lv_obj_set_style_bg_color(obj, lv_color_hex(0xffffff), LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_radius(obj, 30, LV_PART_MAIN | LV_STATE_DEFAULT);
            {
                lv_obj_t *parent_obj = obj;
                {
                    // ai_send_btn
                    lv_obj_t *obj = lv_btn_create(parent_obj);
                    objects.ai_send_btn = obj;
                    lv_obj_set_pos(obj, 351, -9);
                    lv_obj_set_size(obj, 44, 42);
                    lv_obj_set_style_bg_color(obj, lv_color_hex(0xaaaaaa), LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_bg_opa(obj, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_bg_color(obj, lv_color_hex(0x000000), LV_PART_MAIN | LV_STATE_PRESSED);
                    lv_obj_set_style_bg_opa(obj, 255, LV_PART_MAIN | LV_STATE_PRESSED);
                    {
                        lv_obj_t *parent_obj = obj;
                        {
                            lv_obj_t *obj = lv_label_create(parent_obj);
                            lv_obj_set_pos(obj, 0, 0);
                            lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                            lv_obj_set_style_align(obj, LV_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_label_set_text_static(obj, "send");
                        }
                    }
                }
                {
                    // ai_chatbox_shutbtn
                    lv_obj_t *obj = lv_btn_create(parent_obj);
                    objects.ai_chatbox_shutbtn = obj;
                    lv_obj_set_pos(obj, -13, -14);
                    lv_obj_set_size(obj, 58, 50);
                    add_style_checkstyle(obj);
                    lv_obj_set_style_bg_img_src(obj, &img_ai, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_bg_opa(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_bg_opa(obj, 255, LV_PART_MAIN | LV_STATE_CHECKED | LV_STATE_PRESSED);
                    lv_obj_set_style_bg_color(obj, lv_color_hex(0x8f8f8f), LV_PART_MAIN | LV_STATE_CHECKED | LV_STATE_PRESSED);
                    lv_obj_set_style_bg_opa(obj, 255, LV_PART_MAIN | LV_STATE_PRESSED);
                    lv_obj_set_style_bg_color(obj, lv_color_hex(0xa5a5a5), LV_PART_MAIN | LV_STATE_PRESSED);
                    lv_obj_set_style_bg_opa(obj, 0, LV_PART_MAIN | LV_STATE_FOCUSED);
                }
                {
                    // ai_userrequest
                    lv_obj_t *obj = lv_textarea_create(parent_obj);
                    objects.ai_userrequest = obj;
                    lv_obj_set_pos(obj, 51, -9);
                    lv_obj_set_size(obj, 289, 41);
                    lv_textarea_set_max_length(obj, 128);
                    lv_textarea_set_placeholder_text(obj, "Hi,where we going?");
                    lv_textarea_set_one_line(obj, false);
                    lv_textarea_set_password_mode(obj, false);
                    lv_obj_clear_flag(obj, LV_OBJ_FLAG_SCROLLABLE);
                }
            }
        }
        {
            // wealth_panel
            lv_obj_t *obj = lv_obj_create(parent_obj);
            objects.wealth_panel = obj;
            lv_obj_set_pos(obj, 545, 15);
            lv_obj_set_size(obj, 245, 294);
            lv_obj_clear_flag(obj, LV_OBJ_FLAG_SCROLLABLE);
            lv_obj_set_style_radius(obj, 50, LV_PART_MAIN | LV_STATE_CHECKED);
            {
                lv_obj_t *parent_obj = obj;
                {
                    // l3
                    lv_obj_t *obj = lv_line_create(parent_obj);
                    objects.l3 = obj;
                    lv_obj_set_pos(obj, -9, 177);
                    lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                    static lv_point_t line_points[] = {
                        { 0, 20 },
                        { 210, 20 }
                    };
                    lv_line_set_points(obj, line_points, 2);
                    lv_obj_set_style_line_width(obj, 2, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_line_color(obj, lv_color_hex(0x787878), LV_PART_MAIN | LV_STATE_DEFAULT);
                }
                {
                    // day2label
                    lv_obj_t *obj = lv_label_create(parent_obj);
                    objects.day2label = obj;
                    lv_obj_set_pos(obj, 53, 153);
                    lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                    lv_obj_set_style_text_font(obj, &lv_font_montserrat_18, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_label_set_text_static(obj, "Text");
                }
                {
                    // l2
                    lv_obj_t *obj = lv_line_create(parent_obj);
                    objects.l2 = obj;
                    lv_obj_set_pos(obj, -6, 103);
                    lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                    static lv_point_t line_points[] = {
                        { 0, 20 },
                        { 210, 20 }
                    };
                    lv_line_set_points(obj, line_points, 2);
                    lv_obj_set_style_line_width(obj, 2, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_line_color(obj, lv_color_hex(0x787878), LV_PART_MAIN | LV_STATE_DEFAULT);
                }
                {
                    // l1
                    lv_obj_t *obj = lv_line_create(parent_obj);
                    objects.l1 = obj;
                    lv_obj_set_pos(obj, -1, 28);
                    lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                    static lv_point_t line_points[] = {
                        { 0, 20 },
                        { 210, 20 }
                    };
                    lv_line_set_points(obj, line_points, 2);
                    lv_obj_set_style_line_width(obj, 2, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_line_color(obj, lv_color_hex(0x787878), LV_PART_MAIN | LV_STATE_DEFAULT);
                }
                {
                    // day3label
                    lv_obj_t *obj = lv_label_create(parent_obj);
                    objects.day3label = obj;
                    lv_obj_set_pos(obj, 53, 227);
                    lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                    lv_obj_set_style_text_font(obj, &lv_font_montserrat_18, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_label_set_text_static(obj, "Text");
                }
                {
                    // day1label
                    lv_obj_t *obj = lv_label_create(parent_obj);
                    objects.day1label = obj;
                    lv_obj_set_pos(obj, 53, 83);
                    lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                    lv_obj_set_style_text_font(obj, &lv_font_montserrat_18, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_label_set_text_static(obj, "Text");
                }
                {
                    // WuhanLabel
                    lv_obj_t *obj = lv_label_create(parent_obj);
                    objects.wuhan_label = obj;
                    lv_obj_set_pos(obj, 45, -1);
                    lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                    lv_obj_set_style_text_font(obj, &lv_font_montserrat_26, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_label_set_text_static(obj, "Wu Han");
                }
                {
                    // day3icon
                    lv_obj_t *obj = lv_img_create(parent_obj);
                    objects.day3icon = obj;
                    lv_obj_set_pos(obj, -9, 200);
                    lv_obj_set_size(obj, 54, 54);
                }
                {
                    // day2icon
                    lv_obj_t *obj = lv_img_create(parent_obj);
                    objects.day2icon = obj;
                    lv_obj_set_pos(obj, -9, 123);
                    lv_obj_set_size(obj, 54, 54);
                }
                {
                    // day1icon
                    lv_obj_t *obj = lv_img_create(parent_obj);
                    objects.day1icon = obj;
                    lv_obj_set_pos(obj, -9, 53);
                    lv_obj_set_size(obj, 54, 54);
                    lv_img_set_src(obj, &img_wealth);
                }
            }
        }
        {
            // calander
            lv_obj_t *obj = lv_calendar_create(parent_obj);
            objects.calander = obj;
            lv_obj_set_pos(obj, 692, 130);
            lv_obj_set_size(obj, 322, 242);
            lv_calendar_header_arrow_create(obj);
            lv_calendar_set_today_date(obj, 2022, 11, 1);
            lv_calendar_set_showed_date(obj, 2022, 11);
        }
        {
            // songlist
            lv_obj_t *obj = lv_roller_create(parent_obj);
            objects.songlist = obj;
            lv_obj_set_pos(obj, 331, 468);
            lv_obj_set_size(obj, 133, 117);
            lv_roller_set_options(obj, "Option 1\nOption 2\nOption 3", LV_ROLLER_MODE_NORMAL);
            lv_obj_set_style_arc_width(obj, 20, LV_PART_MAIN | LV_STATE_CHECKED | LV_STATE_PRESSED);
            lv_obj_set_style_text_font(obj, &lv_font_montserrat_12, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_bg_color(obj, lv_color_hex(0x7f7f7f), LV_PART_SELECTED | LV_STATE_DEFAULT);
        }
        {
            // sound_btn
            lv_obj_t *obj = lv_btn_create(parent_obj);
            objects.sound_btn = obj;
            lv_obj_set_pos(obj, 951, 544);
            lv_obj_set_size(obj, 52, 48);
            lv_obj_add_flag(obj, LV_OBJ_FLAG_CHECKABLE);
            lv_obj_clear_flag(obj, LV_OBJ_FLAG_CLICK_FOCUSABLE);
            add_style_click_style(obj);
            lv_obj_set_style_bg_img_src(obj, &img_sound, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_bg_img_opa(obj, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_bg_img_recolor(obj, lv_color_hex(0x545454), LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_bg_img_recolor_opa(obj, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_bg_color(obj, lv_color_hex(0x454545), LV_PART_MAIN | LV_STATE_CHECKED);
            lv_obj_set_style_radius(obj, 15, LV_PART_MAIN | LV_STATE_CHECKED);
            lv_obj_set_style_bg_img_recolor(obj, lv_color_hex(0xdddddd), LV_PART_MAIN | LV_STATE_CHECKED);
            lv_obj_set_style_bg_img_recolor_opa(obj, 255, LV_PART_MAIN | LV_STATE_CHECKED);
            lv_obj_set_style_bg_opa(obj, 0, LV_PART_MAIN | LV_STATE_FOCUSED);
        }
        {
            // sound_bar
            lv_obj_t *obj = lv_slider_create(parent_obj);
            objects.sound_bar = obj;
            lv_obj_set_pos(obj, 969, 423);
            lv_obj_set_size(obj, 8, 115);
            lv_slider_set_value(obj, 25, LV_ANIM_ON);
            add_style_darkslider(obj);
        }
        {
            // brightness_btn
            lv_obj_t *obj = lv_btn_create(parent_obj);
            objects.brightness_btn = obj;
            lv_obj_set_pos(obj, 887, 543);
            lv_obj_set_size(obj, 52, 48);
            lv_obj_add_flag(obj, LV_OBJ_FLAG_CHECKABLE);
            lv_obj_clear_flag(obj, LV_OBJ_FLAG_CLICK_FOCUSABLE);
            add_style_click_style(obj);
            lv_obj_set_style_bg_img_src(obj, &img_brightness, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_bg_img_opa(obj, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_bg_img_recolor(obj, lv_color_hex(0x545454), LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_bg_img_recolor_opa(obj, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_bg_color(obj, lv_color_hex(0x454545), LV_PART_MAIN | LV_STATE_CHECKED);
            lv_obj_set_style_radius(obj, 15, LV_PART_MAIN | LV_STATE_CHECKED);
            lv_obj_set_style_bg_img_recolor(obj, lv_color_hex(0xdddddd), LV_PART_MAIN | LV_STATE_CHECKED);
            lv_obj_set_style_bg_img_recolor_opa(obj, 255, LV_PART_MAIN | LV_STATE_CHECKED);
            lv_obj_set_style_bg_img_src(obj, &img_brightness, LV_PART_MAIN | LV_STATE_CHECKED);
            lv_obj_set_style_bg_img_opa(obj, 255, LV_PART_MAIN | LV_STATE_FOCUSED);
        }
        {
            // brightness_bar
            lv_obj_t *obj = lv_slider_create(parent_obj);
            objects.brightness_bar = obj;
            lv_obj_set_pos(obj, 909, 423);
            lv_obj_set_size(obj, 8, 115);
            lv_slider_set_range(obj, 10, 255);
            lv_slider_set_value(obj, 25, LV_ANIM_ON);
            add_style_darkslider(obj);
        }
        {
            // ai_chatkeyboard
            lv_obj_t *obj = lv_keyboard_create(parent_obj);
            objects.ai_chatkeyboard = obj;
            lv_obj_set_pos(obj, 143, 197);
            lv_obj_set_size(obj, 775, 311);
            lv_obj_add_flag(obj, LV_OBJ_FLAG_HIDDEN);
            lv_obj_set_style_align(obj, LV_ALIGN_DEFAULT, LV_PART_MAIN | LV_STATE_DEFAULT);
        }
        {
            // mapin_btn
            lv_obj_t *obj = lv_btn_create(parent_obj);
            objects.mapin_btn = obj;
            lv_obj_set_pos(obj, 953, 126);
            lv_obj_set_size(obj, 58, 58);
            lv_obj_add_flag(obj, LV_OBJ_FLAG_CHECKABLE);
            lv_obj_clear_flag(obj, LV_OBJ_FLAG_CLICK_FOCUSABLE);
            add_style_checkstyle(obj);
            lv_obj_set_style_bg_img_src(obj, &img_carstate, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_bg_img_recolor(obj, lv_color_hex(0x5f5f5f), LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_bg_img_src(obj, &img_carstate, LV_PART_MAIN | LV_STATE_CHECKED);
            lv_obj_set_style_bg_img_recolor(obj, lv_color_hex(0xffffff), LV_PART_MAIN | LV_STATE_CHECKED);
            lv_obj_set_style_bg_color(obj, lv_color_hex(0x595959), LV_PART_MAIN | LV_STATE_CHECKED);
            lv_obj_set_style_bg_opa(obj, 255, LV_PART_MAIN | LV_STATE_CHECKED);
        }
    }
    
    tick_screen_main();
}

void tick_screen_main() {
    screen_main_state_t *state = &screen_main_state;
    (void)state;
}

void create_screen_map() {
    lv_obj_t *obj = lv_obj_create(0);
    objects.map = obj;
    lv_obj_set_pos(obj, 0, 0);
    lv_obj_set_size(obj, 1024, 600);
    {
        lv_obj_t *parent_obj = obj;
        {
            // map_contain
            lv_obj_t *obj = lv_obj_create(parent_obj);
            objects.map_contain = obj;
            lv_obj_set_pos(obj, 347, 0);
            lv_obj_set_size(obj, 677, 600);
            lv_obj_set_style_pad_left(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_pad_top(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_pad_right(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_pad_bottom(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_bg_opa(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_border_width(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_radius(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
        }
        {
            // map_img
            lv_obj_t *obj = lv_img_create(parent_obj);
            objects.map_img = obj;
            lv_obj_set_pos(obj, 347, 0);
            lv_obj_set_size(obj, 677, 600);
        }
        {
            lv_obj_t *obj = lv_obj_create(parent_obj);
            objects.obj1 = obj;
            lv_obj_set_pos(obj, 0, 0);
            lv_obj_set_size(obj, 347, 600);
            lv_obj_clear_flag(obj, LV_OBJ_FLAG_SCROLLABLE);
            lv_obj_set_style_shadow_width(obj, 1, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_shadow_color(obj, lv_color_hex(0x000000), LV_PART_MAIN | LV_STATE_DEFAULT);
            {
                lv_obj_t *parent_obj = obj;
                {
                    lv_obj_t *obj = lv_img_create(parent_obj);
                    lv_obj_set_pos(obj, -38, 28);
                    lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                    lv_img_set_src(obj, &img_carmap);
                    lv_obj_clear_flag(obj, LV_OBJ_FLAG_SCROLLABLE|LV_OBJ_FLAG_SCROLL_ELASTIC);
                }
            }
        }
        {
            lv_obj_t *obj = lv_label_create(parent_obj);
            objects.obj2 = obj;
            lv_obj_set_pos(obj, 25, 25);
            lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
            lv_obj_set_style_text_opa(obj, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_text_color(obj, lv_color_hex(0x646464), LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_text_font(obj, &lv_font_montserrat_46, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_label_set_text_static(obj, "55");
        }
        {
            lv_obj_t *obj = lv_label_create(parent_obj);
            objects.obj3 = obj;
            lv_obj_set_pos(obj, 25, 75);
            lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
            lv_obj_set_style_text_opa(obj, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_text_color(obj, lv_color_hex(0x646464), LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_text_font(obj, &lv_font_montserrat_20, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_label_set_text_static(obj, "KM/H");
        }
        {
            lv_obj_t *obj = lv_obj_create(parent_obj);
            objects.obj4 = obj;
            lv_obj_set_pos(obj, 246, 25);
            lv_obj_set_size(obj, 80, 81);
            lv_obj_clear_flag(obj, LV_OBJ_FLAG_CLICKABLE|LV_OBJ_FLAG_SCROLLABLE);
            lv_obj_set_style_shadow_width(obj, 4, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_shadow_color(obj, lv_color_hex(0xacacac), LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_shadow_opa(obj, 100, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_shadow_spread(obj, 5, LV_PART_MAIN | LV_STATE_DEFAULT);
            {
                lv_obj_t *parent_obj = obj;
                {
                    lv_obj_t *obj = lv_label_create(parent_obj);
                    objects.obj5 = obj;
                    lv_obj_set_pos(obj, -15, -16);
                    lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                    lv_obj_set_style_text_font(obj, &lv_font_montserrat_20, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_border_color(obj, lv_color_hex(0x9e9e9e), LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_border_opa(obj, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_text_opa(obj, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_text_color(obj, lv_color_hex(0x646464), LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_label_set_text_static(obj, "SPEED\n LIMIT\n    80");
                }
            }
        }
        {
            // exit_map_btn
            lv_obj_t *obj = lv_btn_create(parent_obj);
            objects.exit_map_btn = obj;
            lv_obj_set_pos(obj, 952, 6);
            lv_obj_set_size(obj, 66, 60);
            lv_obj_add_state(obj, LV_STATE_FOCUSED);
            add_style_click_style(obj);
            lv_obj_set_style_bg_img_src(obj, &img_exitmap, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_bg_img_opa(obj, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_bg_img_recolor(obj, lv_color_hex(0x696969), LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_bg_img_recolor_opa(obj, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_bg_color(obj, lv_color_hex(0x595959), LV_PART_MAIN | LV_STATE_PRESSED);
            lv_obj_set_style_bg_opa(obj, 255, LV_PART_MAIN | LV_STATE_PRESSED);
            lv_obj_set_style_bg_img_opa(obj, 255, LV_PART_MAIN | LV_STATE_PRESSED);
            lv_obj_set_style_bg_img_recolor(obj, lv_color_hex(0xffffff), LV_PART_MAIN | LV_STATE_PRESSED);
            lv_obj_set_style_bg_img_recolor_opa(obj, 255, LV_PART_MAIN | LV_STATE_PRESSED);
            lv_obj_set_style_bg_opa(obj, 0, LV_PART_MAIN | LV_STATE_CHECKED | LV_STATE_PRESSED);
            lv_obj_set_style_bg_color(obj, lv_color_hex(0x2196f3), LV_PART_MAIN | LV_STATE_CHECKED | LV_STATE_PRESSED);
            lv_obj_set_style_bg_opa(obj, 0, LV_PART_MAIN | LV_STATE_FOCUSED);
            lv_obj_set_style_bg_color(obj, lv_color_hex(0x2196f3), LV_PART_MAIN | LV_STATE_FOCUSED);
        }
    }
    
    tick_screen_map();
}

void tick_screen_map() {
}

typedef void (*tick_screen_func_t)();
tick_screen_func_t tick_screen_funcs[] = {
    tick_screen_main,
    tick_screen_map,
};
void tick_screen(int screen_index) {
    if (screen_index >= 0 && screen_index < 2) {
        tick_screen_funcs[screen_index]();
    }
}
void tick_screen_by_id(enum ScreensEnum screenId) {
    tick_screen(screenId - 1);
}

//
// Fonts
//

ext_font_desc_t fonts[] = {
    { "MI_regular", &ui_font_mi_regular },
#if LV_FONT_MONTSERRAT_8
    { "MONTSERRAT_8", &lv_font_montserrat_8 },
#endif
#if LV_FONT_MONTSERRAT_10
    { "MONTSERRAT_10", &lv_font_montserrat_10 },
#endif
#if LV_FONT_MONTSERRAT_12
    { "MONTSERRAT_12", &lv_font_montserrat_12 },
#endif
#if LV_FONT_MONTSERRAT_14
    { "MONTSERRAT_14", &lv_font_montserrat_14 },
#endif
#if LV_FONT_MONTSERRAT_16
    { "MONTSERRAT_16", &lv_font_montserrat_16 },
#endif
#if LV_FONT_MONTSERRAT_18
    { "MONTSERRAT_18", &lv_font_montserrat_18 },
#endif
#if LV_FONT_MONTSERRAT_20
    { "MONTSERRAT_20", &lv_font_montserrat_20 },
#endif
#if LV_FONT_MONTSERRAT_22
    { "MONTSERRAT_22", &lv_font_montserrat_22 },
#endif
#if LV_FONT_MONTSERRAT_24
    { "MONTSERRAT_24", &lv_font_montserrat_24 },
#endif
#if LV_FONT_MONTSERRAT_26
    { "MONTSERRAT_26", &lv_font_montserrat_26 },
#endif
#if LV_FONT_MONTSERRAT_28
    { "MONTSERRAT_28", &lv_font_montserrat_28 },
#endif
#if LV_FONT_MONTSERRAT_30
    { "MONTSERRAT_30", &lv_font_montserrat_30 },
#endif
#if LV_FONT_MONTSERRAT_32
    { "MONTSERRAT_32", &lv_font_montserrat_32 },
#endif
#if LV_FONT_MONTSERRAT_34
    { "MONTSERRAT_34", &lv_font_montserrat_34 },
#endif
#if LV_FONT_MONTSERRAT_36
    { "MONTSERRAT_36", &lv_font_montserrat_36 },
#endif
#if LV_FONT_MONTSERRAT_38
    { "MONTSERRAT_38", &lv_font_montserrat_38 },
#endif
#if LV_FONT_MONTSERRAT_40
    { "MONTSERRAT_40", &lv_font_montserrat_40 },
#endif
#if LV_FONT_MONTSERRAT_42
    { "MONTSERRAT_42", &lv_font_montserrat_42 },
#endif
#if LV_FONT_MONTSERRAT_44
    { "MONTSERRAT_44", &lv_font_montserrat_44 },
#endif
#if LV_FONT_MONTSERRAT_46
    { "MONTSERRAT_46", &lv_font_montserrat_46 },
#endif
#if LV_FONT_MONTSERRAT_48
    { "MONTSERRAT_48", &lv_font_montserrat_48 },
#endif
};

//
// Color themes
//

uint32_t active_theme_index = 0;

//
//
//

void create_screens() {

// Set default LVGL theme
    lv_disp_t *dispp = lv_disp_get_default();
    lv_theme_t *theme = lv_theme_default_init(dispp, lv_palette_main(LV_PALETTE_BLUE), lv_palette_main(LV_PALETTE_RED), false, LV_FONT_DEFAULT);
    lv_disp_set_theme(dispp, theme);
    
    // Initialize screens
    // Create screens
    create_screen_main();
    create_screen_map();
}