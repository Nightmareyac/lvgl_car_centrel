

#include "anim.h"

#define PANEL_HIDE_X (-300)

//将面板从左侧滑入到目标 x 坐标
void slide_in_panel(lv_obj_t *panel, int x_target, int duration_ms)
{
    if (!panel) return;

    //先取消隐藏，再将面板移到起始位置
    lv_obj_clear_flag(panel, LV_OBJ_FLAG_HIDDEN);
    lv_obj_set_x(panel, PANEL_HIDE_X);

    //配置动画：从左侧外部滑入到目标 x
    lv_anim_t a;
    lv_anim_init(&a);
    lv_anim_set_var(&a, panel);
    lv_anim_set_values(&a, PANEL_HIDE_X, x_target);
    lv_anim_set_time(&a, duration_ms);
    lv_anim_set_path_cb(&a, lv_anim_path_ease_in_out);
    lv_anim_set_exec_cb(&a, (lv_anim_exec_xcb_t)lv_obj_set_x);
    lv_anim_start(&a);
}

//将面板从起始位置滑出到左侧隐藏
void slide_out_panel(lv_obj_t *panel, int x_start, int duration_ms, lv_anim_ready_cb_t ready_cb)
{
    if (!panel) return;

    //配置动画：从起始位置滑出到左侧外部
    lv_anim_t a;
    lv_anim_init(&a);
    lv_anim_set_var(&a, panel);
    lv_anim_set_values(&a, x_start, PANEL_HIDE_X);
    lv_anim_set_time(&a, duration_ms);
    lv_anim_set_path_cb(&a, lv_anim_path_ease_in);  /* 缓入曲线（退出时减速） */
    lv_anim_set_exec_cb(&a, (lv_anim_exec_xcb_t)lv_obj_set_x);

    if (ready_cb) {
        lv_anim_set_ready_cb(&a, ready_cb);
    }

    lv_anim_start(&a);
}
