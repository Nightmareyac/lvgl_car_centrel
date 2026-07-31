/**
 * @file    anim.c
 * @brief   通用 LVGL 面板动画辅助函数实现
 * @details 封装了 LVGL 动画 API，提供两组通用能力：
 *          1. slide_in_panel  — 从左侧外部滑入到目标位置（展开面板）
 *          2. slide_out_panel — 从当前位置滑出到左侧外部（收起面板）
 * @note    所有模块（音乐播放器、天气面板、空调面板）均可复用，不依赖任何业务状态。
 */

#include "anim.h"

/* 面板隐藏时的 x 坐标（屏幕左侧外部） */
#define PANEL_HIDE_X (-300)

/**
 * @brief 将面板从左侧滑入到目标 x 坐标
 * @param panel       要动画的 LVGL 面板对象
 * @param x_target    目标 x 坐标（像素）
 * @param duration_ms 动画时长（毫秒）
 */
void slide_in_panel(lv_obj_t *panel, int x_target, int duration_ms)
{
    if (!panel) return;

    /* 先取消隐藏，再将面板移到起始位置 */
    lv_obj_clear_flag(panel, LV_OBJ_FLAG_HIDDEN);
    lv_obj_set_x(panel, PANEL_HIDE_X);

    /* 配置动画：从左侧外部滑入到目标 x */
    lv_anim_t a;
    lv_anim_init(&a);
    lv_anim_set_var(&a, panel);
    lv_anim_set_values(&a, PANEL_HIDE_X, x_target);
    lv_anim_set_time(&a, duration_ms);
    lv_anim_set_path_cb(&a, lv_anim_path_ease_in_out);  /* 缓入缓出曲线 */
    lv_anim_set_exec_cb(&a, (lv_anim_exec_xcb_t)lv_obj_set_x);
    lv_anim_start(&a);
}

/**
 * @brief 将面板从起始位置滑出到左侧隐藏
 * @param panel       要动画的 LVGL 面板对象
 * @param x_start     起始 x 坐标（像素）
 * @param duration_ms 动画时长（毫秒）
 * @param ready_cb    动画结束回调（可选，传 NULL 则无回调）
 */
void slide_out_panel(lv_obj_t *panel, int x_start, int duration_ms, lv_anim_ready_cb_t ready_cb)
{
    if (!panel) return;

    /* 配置动画：从起始位置滑出到左侧外部 */
    lv_anim_t a;
    lv_anim_init(&a);
    lv_anim_set_var(&a, panel);
    lv_anim_set_values(&a, x_start, PANEL_HIDE_X);
    lv_anim_set_time(&a, duration_ms);
    lv_anim_set_path_cb(&a, lv_anim_path_ease_in);  /* 缓入曲线（退出时减速） */
    lv_anim_set_exec_cb(&a, (lv_anim_exec_xcb_t)lv_obj_set_x);

    /* 可选：动画结束时回调（常用于隐藏面板或恢复状态） */
    if (ready_cb) {
        lv_anim_set_ready_cb(&a, ready_cb);
    }

    lv_anim_start(&a);
}
