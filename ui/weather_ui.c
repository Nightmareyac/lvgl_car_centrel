/**
 * @file    weather_ui.c
 * @brief   天气面板 UI 模块实现
 * @details 封装了天气面板和日历面板的 UI 交互：
 *          - 天气按钮：点击后滑入天气面板，同时启动后台 pthread 获取天气数据
 *          - 日历按钮：点击后切换日历面板的显示/隐藏
 * @note    1. 天气数据获取在后台线程执行（weather_thread_func），避免阻塞 UI。
 *          2. 保留了 EEZ Studio 生成的原始对象名（如 wealth_panel、calander）。
 */

#include "weather_ui.h"
#include "screens.h"
#include "timePanel.h"
#include "anim.h"

#include <pthread.h>
#include <stdio.h>

/* 天气面板的目标 x 坐标（滑入后的位置） */
#define WEALTH_PANEL_TARGET_X  540
/* 动画时长（毫秒） */
#define WEALTH_SLIDE_IN_MS     350
#define WEALTH_SLIDE_OUT_MS    300

/* ------------------------------------------------------------------ */
/* 内部回调函数                                                          */
/* ------------------------------------------------------------------ */

/**
 * @brief 天气面板收起动画结束回调 — 隐藏天气面板
 */
static void wealth_collapse_ready_cb(lv_anim_t *a)
{
    lv_obj_add_flag(objects.wealth_panel, LV_OBJ_FLAG_HIDDEN);
}

/* ------------------------------------------------------------------ */
/* 事件回调                                                              */
/* ------------------------------------------------------------------ */

/**
 * @brief 天气按钮回调
 * @details 开启时：滑入天气面板 + 启动后台线程获取天气数据。
 *          关闭时：滑出天气面板（动画结束后自动隐藏）。
 */
void wealth_btn_cb(lv_event_t *e)
{
    if (!objects.wealth_panel) return;

    lv_obj_t *btn = lv_event_get_target(e);

    if (lv_obj_has_state(btn, LV_STATE_CHECKED)) {
        /* 展开天气面板 */
        slide_in_panel(objects.wealth_panel, WEALTH_PANEL_TARGET_X, WEALTH_SLIDE_IN_MS);

        /* 启动后台线程获取天气数据（线程分离，结束后自动回收资源） */
        pthread_t thread_id;
        pthread_create(&thread_id, NULL, weather_thread_func, NULL);
        pthread_detach(thread_id);
    } else {
        /* 收起天气面板 */
        slide_out_panel(objects.wealth_panel, WEALTH_PANEL_TARGET_X,
                        WEALTH_SLIDE_OUT_MS, wealth_collapse_ready_cb);
    }
}

/**
 * @brief 日历按钮回调 — 切换日历面板的显示/隐藏
 */
void calander_btn_cb(lv_event_t *e)
{
    if (!objects.calander) return;

    lv_obj_t *btn = lv_event_get_target(e);
    if (lv_obj_has_state(btn, LV_STATE_CHECKED)) {
        lv_obj_clear_flag(objects.calander, LV_OBJ_FLAG_HIDDEN);
    } else {
        lv_obj_add_flag(objects.calander, LV_OBJ_FLAG_HIDDEN);
    }
}

/* ------------------------------------------------------------------ */
/* 模块初始化                                                           */
/* ------------------------------------------------------------------ */

/**
 * @brief 初始化天气面板 UI 模块
 * @details 1. 设置天气面板和日历面板默认隐藏
 *          2. 绑定天气按钮和日历按钮的事件回调
 */
void weather_ui_init(void)
{
    /* 默认隐藏天气面板和日历面板 */
    if (objects.wealth_panel)
        lv_obj_add_flag(objects.wealth_panel, LV_OBJ_FLAG_HIDDEN);
    if (objects.calander)
        lv_obj_add_flag(objects.calander, LV_OBJ_FLAG_HIDDEN);

    /* 绑定天气按钮（可切换状态） */
    if (objects.wealth_btn) {
        lv_obj_add_flag(objects.wealth_btn, LV_OBJ_FLAG_CHECKABLE);
        lv_obj_add_event_cb(objects.wealth_btn, wealth_btn_cb, LV_EVENT_CLICKED, NULL);
    }

    /* 绑定日历按钮（可切换状态） */
    if (objects.calander_btn) {
        lv_obj_add_flag(objects.calander_btn, LV_OBJ_FLAG_CHECKABLE);
        lv_obj_add_event_cb(objects.calander_btn, calander_btn_cb, LV_EVENT_CLICKED, NULL);
    }
}
