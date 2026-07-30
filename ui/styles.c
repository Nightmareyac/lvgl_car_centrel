#include "styles.h"
#include "images.h"
#include "fonts.h"

#include "ui.h"
#include "screens.h"

//
// Style: click_style
//

void init_style_click_style_MAIN_DEFAULT(lv_style_t *style) {
    lv_style_set_radius(style, 15);
    lv_style_set_bg_opa(style, 0);
    lv_style_set_shadow_opa(style, 0);
    lv_style_set_text_color(style, lv_color_hex(0x000000));
    lv_style_set_text_opa(style, 255);
};

lv_style_t *get_style_click_style_MAIN_DEFAULT() {
    static lv_style_t *style;
    if (!style) {
        style = (lv_style_t *)lv_mem_alloc(sizeof(lv_style_t));
        lv_style_init(style);
        init_style_click_style_MAIN_DEFAULT(style);
    }
    return style;
};

void init_style_click_style_MAIN_PRESSED(lv_style_t *style) {
    lv_style_set_bg_color(style, lv_color_hex(0x3d3d3d));
    lv_style_set_bg_opa(style, 255);
    lv_style_set_shadow_opa(style, 0);
    lv_style_set_radius(style, 10);
};

lv_style_t *get_style_click_style_MAIN_PRESSED() {
    static lv_style_t *style;
    if (!style) {
        style = (lv_style_t *)lv_mem_alloc(sizeof(lv_style_t));
        lv_style_init(style);
        init_style_click_style_MAIN_PRESSED(style);
    }
    return style;
};

void init_style_click_style_MAIN_FOCUSED(lv_style_t *style) {
    lv_style_set_bg_opa(style, 0);
    lv_style_set_bg_color(style, lv_color_hex(0xffffff));
};

lv_style_t *get_style_click_style_MAIN_FOCUSED() {
    static lv_style_t *style;
    if (!style) {
        style = (lv_style_t *)lv_mem_alloc(sizeof(lv_style_t));
        lv_style_init(style);
        init_style_click_style_MAIN_FOCUSED(style);
    }
    return style;
};

void init_style_click_style_MAIN_CHECKED(lv_style_t *style) {
    lv_style_set_bg_color(style, lv_color_hex(0x3b3b3b));
    lv_style_set_bg_opa(style, 255);
    lv_style_set_text_color(style, lv_color_hex(0xffffff));
    lv_style_set_text_opa(style, 255);
};

lv_style_t *get_style_click_style_MAIN_CHECKED() {
    static lv_style_t *style;
    if (!style) {
        style = (lv_style_t *)lv_mem_alloc(sizeof(lv_style_t));
        lv_style_init(style);
        init_style_click_style_MAIN_CHECKED(style);
    }
    return style;
};

void init_style_click_style_MAIN_CHECKED_PRESSED(lv_style_t *style) {
    lv_style_set_text_color(style, lv_color_hex(0xffffff));
    lv_style_set_text_opa(style, 255);
};

lv_style_t *get_style_click_style_MAIN_CHECKED_PRESSED() {
    static lv_style_t *style;
    if (!style) {
        style = (lv_style_t *)lv_mem_alloc(sizeof(lv_style_t));
        lv_style_init(style);
        init_style_click_style_MAIN_CHECKED_PRESSED(style);
    }
    return style;
};

void add_style_click_style(lv_obj_t *obj) {
    (void)obj;
    lv_obj_add_style(obj, get_style_click_style_MAIN_DEFAULT(), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_add_style(obj, get_style_click_style_MAIN_PRESSED(), LV_PART_MAIN | LV_STATE_PRESSED);
    lv_obj_add_style(obj, get_style_click_style_MAIN_FOCUSED(), LV_PART_MAIN | LV_STATE_FOCUSED);
    lv_obj_add_style(obj, get_style_click_style_MAIN_CHECKED(), LV_PART_MAIN | LV_STATE_CHECKED);
    lv_obj_add_style(obj, get_style_click_style_MAIN_CHECKED_PRESSED(), LV_PART_MAIN | LV_STATE_CHECKED | LV_STATE_PRESSED);
};

void remove_style_click_style(lv_obj_t *obj) {
    (void)obj;
    lv_obj_remove_style(obj, get_style_click_style_MAIN_DEFAULT(), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_remove_style(obj, get_style_click_style_MAIN_PRESSED(), LV_PART_MAIN | LV_STATE_PRESSED);
    lv_obj_remove_style(obj, get_style_click_style_MAIN_FOCUSED(), LV_PART_MAIN | LV_STATE_FOCUSED);
    lv_obj_remove_style(obj, get_style_click_style_MAIN_CHECKED(), LV_PART_MAIN | LV_STATE_CHECKED);
    lv_obj_remove_style(obj, get_style_click_style_MAIN_CHECKED_PRESSED(), LV_PART_MAIN | LV_STATE_CHECKED | LV_STATE_PRESSED);
};

//
// Style: checkstyle
//

void init_style_checkstyle_MAIN_DEFAULT(lv_style_t *style) {
    init_style_click_style_MAIN_DEFAULT(style);
    
    lv_style_set_bg_img_src(style, &img_lock);
    lv_style_set_bg_img_recolor(style, lv_color_hex(0x000000));
    lv_style_set_bg_img_recolor_opa(style, 255);
};

lv_style_t *get_style_checkstyle_MAIN_DEFAULT() {
    static lv_style_t *style;
    if (!style) {
        style = (lv_style_t *)lv_mem_alloc(sizeof(lv_style_t));
        lv_style_init(style);
        init_style_checkstyle_MAIN_DEFAULT(style);
    }
    return style;
};

void init_style_checkstyle_MAIN_CHECKED(lv_style_t *style) {
    init_style_click_style_MAIN_CHECKED(style);
    
    lv_style_set_bg_opa(style, 0);
    lv_style_set_bg_img_src(style, &img_unlock);
    lv_style_set_bg_img_recolor(style, lv_color_hex(0x363636));
    lv_style_set_bg_color(style, lv_color_hex(0xffffff));
};

lv_style_t *get_style_checkstyle_MAIN_CHECKED() {
    static lv_style_t *style;
    if (!style) {
        style = (lv_style_t *)lv_mem_alloc(sizeof(lv_style_t));
        lv_style_init(style);
        init_style_checkstyle_MAIN_CHECKED(style);
    }
    return style;
};

void init_style_checkstyle_MAIN_PRESSED(lv_style_t *style) {
    init_style_click_style_MAIN_PRESSED(style);
    
    lv_style_set_bg_color(style, lv_color_hex(0x3d3d3d));
    lv_style_set_bg_opa(style, 0);
};

lv_style_t *get_style_checkstyle_MAIN_PRESSED() {
    static lv_style_t *style;
    if (!style) {
        style = (lv_style_t *)lv_mem_alloc(sizeof(lv_style_t));
        lv_style_init(style);
        init_style_checkstyle_MAIN_PRESSED(style);
    }
    return style;
};

void init_style_checkstyle_MAIN_FOCUSED(lv_style_t *style) {
    init_style_click_style_MAIN_FOCUSED(style);
    
    lv_style_set_bg_opa(style, 0);
    lv_style_set_bg_color(style, lv_color_hex(0x000000));
};

lv_style_t *get_style_checkstyle_MAIN_FOCUSED() {
    static lv_style_t *style;
    if (!style) {
        style = (lv_style_t *)lv_mem_alloc(sizeof(lv_style_t));
        lv_style_init(style);
        init_style_checkstyle_MAIN_FOCUSED(style);
    }
    return style;
};

void init_style_checkstyle_MAIN_CHECKED_PRESSED(lv_style_t *style) {
    init_style_click_style_MAIN_CHECKED_PRESSED(style);
    
    lv_style_set_bg_opa(style, 0);
    lv_style_set_bg_color(style, lv_color_hex(0xffffff));
};

lv_style_t *get_style_checkstyle_MAIN_CHECKED_PRESSED() {
    static lv_style_t *style;
    if (!style) {
        style = (lv_style_t *)lv_mem_alloc(sizeof(lv_style_t));
        lv_style_init(style);
        init_style_checkstyle_MAIN_CHECKED_PRESSED(style);
    }
    return style;
};

void add_style_checkstyle(lv_obj_t *obj) {
    (void)obj;
    lv_obj_add_style(obj, get_style_checkstyle_MAIN_DEFAULT(), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_add_style(obj, get_style_checkstyle_MAIN_CHECKED(), LV_PART_MAIN | LV_STATE_CHECKED);
    lv_obj_add_style(obj, get_style_checkstyle_MAIN_PRESSED(), LV_PART_MAIN | LV_STATE_PRESSED);
    lv_obj_add_style(obj, get_style_checkstyle_MAIN_FOCUSED(), LV_PART_MAIN | LV_STATE_FOCUSED);
    lv_obj_add_style(obj, get_style_checkstyle_MAIN_CHECKED_PRESSED(), LV_PART_MAIN | LV_STATE_CHECKED | LV_STATE_PRESSED);
};

void remove_style_checkstyle(lv_obj_t *obj) {
    (void)obj;
    lv_obj_remove_style(obj, get_style_checkstyle_MAIN_DEFAULT(), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_remove_style(obj, get_style_checkstyle_MAIN_CHECKED(), LV_PART_MAIN | LV_STATE_CHECKED);
    lv_obj_remove_style(obj, get_style_checkstyle_MAIN_PRESSED(), LV_PART_MAIN | LV_STATE_PRESSED);
    lv_obj_remove_style(obj, get_style_checkstyle_MAIN_FOCUSED(), LV_PART_MAIN | LV_STATE_FOCUSED);
    lv_obj_remove_style(obj, get_style_checkstyle_MAIN_CHECKED_PRESSED(), LV_PART_MAIN | LV_STATE_CHECKED | LV_STATE_PRESSED);
};

//
// Style: darkslider
//

void init_style_darkslider_MAIN_CHECKED(lv_style_t *style) {
    lv_style_set_flex_flow(style, LV_FLEX_FLOW_ROW);
};

lv_style_t *get_style_darkslider_MAIN_CHECKED() {
    static lv_style_t *style;
    if (!style) {
        style = (lv_style_t *)lv_mem_alloc(sizeof(lv_style_t));
        lv_style_init(style);
        init_style_darkslider_MAIN_CHECKED(style);
    }
    return style;
};

void init_style_darkslider_MAIN_DEFAULT(lv_style_t *style) {
    lv_style_set_bg_color(style, lv_color_hex(0x5c5c5c));
};

lv_style_t *get_style_darkslider_MAIN_DEFAULT() {
    static lv_style_t *style;
    if (!style) {
        style = (lv_style_t *)lv_mem_alloc(sizeof(lv_style_t));
        lv_style_init(style);
        init_style_darkslider_MAIN_DEFAULT(style);
    }
    return style;
};

void init_style_darkslider_INDICATOR_DEFAULT(lv_style_t *style) {
    lv_style_set_bg_color(style, lv_color_hex(0x797979));
};

lv_style_t *get_style_darkslider_INDICATOR_DEFAULT() {
    static lv_style_t *style;
    if (!style) {
        style = (lv_style_t *)lv_mem_alloc(sizeof(lv_style_t));
        lv_style_init(style);
        init_style_darkslider_INDICATOR_DEFAULT(style);
    }
    return style;
};

void init_style_darkslider_KNOB_DEFAULT(lv_style_t *style) {
    lv_style_set_bg_color(style, lv_color_hex(0x555555));
    lv_style_set_bg_opa(style, 255);
};

lv_style_t *get_style_darkslider_KNOB_DEFAULT() {
    static lv_style_t *style;
    if (!style) {
        style = (lv_style_t *)lv_mem_alloc(sizeof(lv_style_t));
        lv_style_init(style);
        init_style_darkslider_KNOB_DEFAULT(style);
    }
    return style;
};

void add_style_darkslider(lv_obj_t *obj) {
    (void)obj;
    lv_obj_add_style(obj, get_style_darkslider_MAIN_CHECKED(), LV_PART_MAIN | LV_STATE_CHECKED);
    lv_obj_add_style(obj, get_style_darkslider_MAIN_DEFAULT(), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_add_style(obj, get_style_darkslider_INDICATOR_DEFAULT(), LV_PART_INDICATOR | LV_STATE_DEFAULT);
    lv_obj_add_style(obj, get_style_darkslider_KNOB_DEFAULT(), LV_PART_KNOB | LV_STATE_DEFAULT);
};

void remove_style_darkslider(lv_obj_t *obj) {
    (void)obj;
    lv_obj_remove_style(obj, get_style_darkslider_MAIN_CHECKED(), LV_PART_MAIN | LV_STATE_CHECKED);
    lv_obj_remove_style(obj, get_style_darkslider_MAIN_DEFAULT(), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_remove_style(obj, get_style_darkslider_INDICATOR_DEFAULT(), LV_PART_INDICATOR | LV_STATE_DEFAULT);
    lv_obj_remove_style(obj, get_style_darkslider_KNOB_DEFAULT(), LV_PART_KNOB | LV_STATE_DEFAULT);
};

//
// Style: FRUNK
//

void init_style_frunk_MAIN_DEFAULT(lv_style_t *style) {
    lv_style_set_bg_img_src(style, &img_car);
};

lv_style_t *get_style_frunk_MAIN_DEFAULT() {
    static lv_style_t *style;
    if (!style) {
        style = (lv_style_t *)lv_mem_alloc(sizeof(lv_style_t));
        lv_style_init(style);
        init_style_frunk_MAIN_DEFAULT(style);
    }
    return style;
};

void add_style_frunk(lv_obj_t *obj) {
    (void)obj;
    lv_obj_add_style(obj, get_style_frunk_MAIN_DEFAULT(), LV_PART_MAIN | LV_STATE_DEFAULT);
};

void remove_style_frunk(lv_obj_t *obj) {
    (void)obj;
    lv_obj_remove_style(obj, get_style_frunk_MAIN_DEFAULT(), LV_PART_MAIN | LV_STATE_DEFAULT);
};

//
//
//

void add_style(lv_obj_t *obj, int32_t styleIndex) {
    typedef void (*AddStyleFunc)(lv_obj_t *obj);
    static const AddStyleFunc add_style_funcs[] = {
        add_style_click_style,
        add_style_checkstyle,
        add_style_darkslider,
        add_style_frunk,
    };
    add_style_funcs[styleIndex](obj);
}

void remove_style(lv_obj_t *obj, int32_t styleIndex) {
    typedef void (*RemoveStyleFunc)(lv_obj_t *obj);
    static const RemoveStyleFunc remove_style_funcs[] = {
        remove_style_click_style,
        remove_style_checkstyle,
        remove_style_darkslider,
        remove_style_frunk,
    };
    remove_style_funcs[styleIndex](obj);
}