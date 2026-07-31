/**
 * @file    anim.h
 * @brief   通用 LVGL 面板动画辅助函数
 * @details 提供滑入/滑出动画，供各 UI 模块（音乐、天气、空调等）复用。
 *          无业务逻辑依赖，仅封装 LVGL 动画 API。
 */

#ifndef UI_ANIM_H
#define UI_ANIM_H

#include <lvgl/lvgl.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief 将面板从左侧滑入到目标 x 坐标
 * @param panel       要动画的 LVGL 面板对象
 * @param x_target    目标 x 坐标（像素）
 * @param duration_ms 动画时长（毫秒）
 * @note  动画前会自动清除 HIDDEN 标志并将起始位置设为 x=-300，使用 ease_in_out 曲线
 */
void slide_in_panel(lv_obj_t *panel, int x_target, int duration_ms);

/**
 * @brief 将面板从起始位置滑出到左侧隐藏
 * @param panel       要动画的 LVGL 面板对象
 * @param x_start     起始 x 坐标（像素）
 * @param duration_ms 动画时长（毫秒）
 * @param ready_cb    动画结束回调（可选，传 NULL 则无回调），常用于隐藏面板
 * @note  使用 ease_in 曲线，终点固定在 x=-300
 */
void slide_out_panel(lv_obj_t *panel, int x_start, int duration_ms, lv_anim_ready_cb_t ready_cb);

#ifdef __cplusplus
}
#endif

#endif /* UI_ANIM_H */
