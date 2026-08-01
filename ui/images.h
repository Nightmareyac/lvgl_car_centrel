#ifndef EEZ_LVGL_UI_IMAGES_H
#define EEZ_LVGL_UI_IMAGES_H

#include <lvgl/lvgl.h>

#ifdef __cplusplus
extern "C" {
#endif

extern const lv_img_dsc_t img_backwardimg;
extern const lv_img_dsc_t img_forwardimg;
extern const lv_img_dsc_t img_pauseimg;
extern const lv_img_dsc_t img_fullimg;
extern const lv_img_dsc_t img_cdimg;
extern const lv_img_dsc_t img_cloudmusice_icon;
extern const lv_img_dsc_t img_fullexit;
extern const lv_img_dsc_t img_playelist;
extern const lv_img_dsc_t img_playimg;
extern const lv_img_dsc_t img_wealth;
extern const lv_img_dsc_t img_car;
extern const lv_img_dsc_t img_sound;
extern const lv_img_dsc_t img_flash1;
extern const lv_img_dsc_t img_flash2;
extern const lv_img_dsc_t img_flash3;
extern const lv_img_dsc_t img_safebelt;
extern const lv_img_dsc_t img_lock;
extern const lv_img_dsc_t img_unlock;
extern const lv_img_dsc_t img_flash;
extern const lv_img_dsc_t img_ai;
extern const lv_img_dsc_t img_brightness;
extern const lv_img_dsc_t img_air;
extern const lv_img_dsc_t img_telsa;
extern const lv_img_dsc_t img_carstate;
extern const lv_img_dsc_t img_carmap;
extern const lv_img_dsc_t img_exitmap;

#ifndef EXT_IMG_DESC_T
#define EXT_IMG_DESC_T
typedef struct _ext_img_desc_t {
    const char *name;
    const lv_img_dsc_t *img_dsc;
} ext_img_desc_t;
#endif

extern const ext_img_desc_t images[26];

#ifdef __cplusplus
}
#endif

#endif /*EEZ_LVGL_UI_IMAGES_H*/