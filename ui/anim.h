
#ifndef UI_ANIM_H
#define UI_ANIM_H

#include <lvgl/lvgl.h>

#ifdef __cplusplus
extern "C" {
#endif


void slide_in_panel(lv_obj_t *panel, int x_target, int duration_ms);

void slide_out_panel(lv_obj_t *panel, int x_start, int duration_ms, lv_anim_ready_cb_t ready_cb);

#ifdef __cplusplus
}
#endif

#endif /* UI_ANIM_H */
