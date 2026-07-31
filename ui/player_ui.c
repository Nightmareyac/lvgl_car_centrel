/**
 * @file    player_ui.c
 * @brief   音乐播放器 UI 模块实现
 * @details 封装了音乐播放器的全部 UI 交互逻辑：
 *          - 播放/暂停/切歌按钮回调
 *          - 歌曲列表（roller）的构建与选择
 *          - 进度条定时更新
 *          - 全屏播放面板的展开/折叠动画
 * @note    1. 底层音频操作委托给 music.c（mplayer 从模式通过 FIFO 控制）。
 *          2. 进度条定时器仅创建一次（修复原 actions.c 中重复创建的 Bug）。
 *          3. 保留了 EEZ Studio 生成的原始对象名（如 backword_btn、upadate_musicUI）。
 */

#include "player_ui.h"
#include "screens.h"
#include "music.h"
#include "anim.h"

#include <stdio.h>
#include <string.h>

/* ------------------------------------------------------------------ */
/* 内部辅助函数声明                                                      */
/* ------------------------------------------------------------------ */

/** 更新音乐 UI 显示（歌名、歌手、封面、进度条、播放状态） */
static void upadate_musicUI(void);

/** 构建歌曲列表 roller 的选项文本（用换行符分隔歌名） */
static void build_songlist(lv_obj_t *roller);

/* ------------------------------------------------------------------ */
/* 内部回调函数                                                          */
/* ------------------------------------------------------------------ */

/**
 * @brief 歌曲列表 roller 选择事件回调
 * @details 用户选择一首歌曲后，跳转到该歌曲并更新 UI，然后隐藏歌曲列表。
 */
static void songlist_item_cb(lv_event_t *e)
{
    lv_obj_t *roller = lv_event_get_target(e);
    uint16_t index = lv_roller_get_selected(roller);

    music_jump_to(index);
    upadate_musicUI();

    /* 选择后隐藏列表并取消按钮选中状态 */
    lv_obj_add_flag(objects.songlist, LV_OBJ_FLAG_HIDDEN);
    lv_obj_clear_state(objects.songlist_btn, LV_STATE_CHECKED);
}

/**
 * @brief 收起动画结束回调 — 隐藏播放大面板和歌曲列表
 */
static void collapes_ready_cb(lv_anim_t *a)
{
    lv_obj_add_flag(objects.player_main_panel, LV_OBJ_FLAG_HIDDEN);
    lv_obj_add_flag(objects.songlist, LV_OBJ_FLAG_HIDDEN);
}

/**
 * @brief 进度条定时器回调（每秒执行一次）
 * @details 仅在音乐播放中时更新进度条值。
 */
static void progress_timer_cb(lv_timer_t *t)
{
    if (objects.progress_bar && music_is_playing()) {
        lv_bar_set_value(objects.progress_bar, music_get_progress(), LV_ANIM_OFF);
    }
}

/* ------------------------------------------------------------------ */
/* 播放控制回调                                                          */
/* ------------------------------------------------------------------ */

/**
 * @brief 播放/暂停按钮回调 — 切换播放状态并刷新 UI
 */
void play_pause_cb(lv_event_t *e)
{
    music_play_toggle();
    upadate_musicUI();
}

/**
 * @brief 下一首按钮回调 — 切换到下一首歌曲并刷新 UI
 */
void next_song_cb(lv_event_t *e)
{
    music_next();
    upadate_musicUI();
}

/**
 * @brief 上一首按钮回调 — 切换到上一首歌曲并刷新 UI
 */
void prev_song_cb(lv_event_t *e)
{
    music_prev();
    upadate_musicUI();
}

/* ------------------------------------------------------------------ */
/* 播放面板展开/折叠                                                     */
/* ------------------------------------------------------------------ */

/**
 * @brief 展开全屏播放面板
 * @details 隐藏小组件面板，将大播放面板从左侧滑入。
 */
void expand_main_panel(lv_event_t *e)
{
    lv_obj_add_flag(objects.player_widget_panel, LV_OBJ_FLAG_HIDDEN);
    slide_in_panel(objects.player_main_panel, 17, 350);
}

/**
 * @brief 收起全屏播放面板
 * @details 将大播放面板滑出到左侧，同时将小组件面板滑入。
 *          动画结束后通过 collapes_ready_cb 隐藏大面板。
 */
void collapse_main_panel(lv_event_t *e)
{
    slide_out_panel(objects.player_main_panel, 17, 300, collapes_ready_cb);
    slide_in_panel(objects.player_widget_panel, 17, 300);
}

/* ------------------------------------------------------------------ */
/* 歌曲列表                                                              */
/* ------------------------------------------------------------------ */

/**
 * @brief 歌曲列表按钮回调 — 切换歌曲列表的显示/隐藏
 */
void songlist_btn_cb(lv_event_t *e)
{
    if (!objects.songlist) {
        printf("[ERROR] objects.songlist is NULL\n");
        return;
    }

    lv_obj_t *btn = lv_event_get_target(e);
    if (lv_obj_has_state(btn, LV_STATE_CHECKED)) {
        lv_obj_clear_flag(objects.songlist, LV_OBJ_FLAG_HIDDEN);
    } else {
        lv_obj_add_flag(objects.songlist, LV_OBJ_FLAG_HIDDEN);
    }
}

/* ------------------------------------------------------------------ */
/* 内部辅助函数实现                                                      */
/* ------------------------------------------------------------------ */

/**
 * @brief 更新音乐 UI 显示
 * @details 从 music.c 获取当前歌曲信息，更新以下 UI 元素：
 *          - 歌名标签（song_name、song_tittle）
 *          - 歌手标签（songer）
 *          - 封面图片（song_panel、song_panel_main）
 *          - 进度条归零
 *          - 播放/暂停按钮状态（两个按钮同步）
 */
static void upadate_musicUI(void)
{
    Song *cur = music_get_current_song();
    if (!cur) return;

    /* 更新歌名（小组件 + 全屏两个位置） */
    if (objects.song_name)
        lv_label_set_text(objects.song_name, cur->tittle);
    if (objects.song_tittle)
        lv_label_set_text(objects.song_tittle, cur->tittle);

    /* 更新歌手 */
    if (objects.songer)
        lv_label_set_text(objects.songer, cur->songer);

    /* 更新封面（主背景用 cover_path1，小组件用 cover_path） */
    if (objects.song_panel_main && cur->cover_path1) {
        lv_obj_set_style_bg_img_src(objects.song_panel_main, cur->cover_path1,
                                    LV_PART_MAIN | LV_STATE_DEFAULT);
    }
    if (objects.song_panel && cur->cover_path) {
        lv_obj_set_style_bg_img_src(objects.song_panel, cur->cover_path,
                                    LV_PART_MAIN | LV_STATE_DEFAULT);
    }

    /* 进度条归零（切歌后重新开始） */
    if (objects.progress_bar)
        lv_bar_set_value(objects.progress_bar, 0, LV_ANIM_OFF);

    /* 同步两个播放/暂停按钮的选中状态 */
    if (music_is_playing()) {
        if (objects.pause_btn)      lv_obj_add_state(objects.pause_btn, LV_STATE_CHECKED);
        if (objects.pause_main_btn) lv_obj_add_state(objects.pause_main_btn, LV_STATE_CHECKED);
    } else {
        if (objects.pause_btn)      lv_obj_clear_state(objects.pause_btn, LV_STATE_CHECKED);
        if (objects.pause_main_btn) lv_obj_clear_state(objects.pause_main_btn, LV_STATE_CHECKED);
    }
}

/**
 * @brief 构建歌曲列表 roller 的选项文本
 * @param roller LVGL roller 对象
 * @details 将所有歌曲名用换行符拼接成一个字符串，设置为 roller 的选项。
 */
static void build_songlist(lv_obj_t *roller)
{
    if (!roller) return;

    char options[512] = "";
    for (int i = 0; i < song_count; i++) {
        if (i > 0) strcat(options, "\n");
        strcat(options, song_list[i].tittle);
    }
    lv_roller_set_options(roller, options, LV_ROLLER_MODE_NORMAL);
}

/* ------------------------------------------------------------------ */
/* 模块初始化                                                           */
/* ------------------------------------------------------------------ */

/**
 * @brief 初始化音乐播放器 UI 模块
 * @details 1. 初始化底层音乐播放器
 *          2. 创建进度条定时器（仅一次）
 *          3. 构建歌曲列表并绑定事件
 *          4. 绑定播放控制按钮事件
 *          5. 设置默认 UI 状态
 */
void player_ui_init(void)
{
    /* 初始化底层音乐播放器（mplayer 从模式） */
    music_init();

    /* 创建进度条更新定时器（每秒刷新一次） */
    lv_timer_create(progress_timer_cb, 1000, NULL);

    /* 默认隐藏全屏播放面板 */
    if (objects.player_main_panel)
        lv_obj_add_flag(objects.player_main_panel, LV_OBJ_FLAG_HIDDEN);

    /* 构建歌曲列表并绑定 roller 事件 */
    if (objects.songlist) {
        build_songlist(objects.songlist);
        lv_obj_add_flag(objects.songlist, LV_OBJ_FLAG_HIDDEN);
        lv_obj_add_event_cb(objects.songlist, songlist_item_cb, LV_EVENT_VALUE_CHANGED, NULL);
    }

    /* 初始化当前歌曲 UI 显示 */
    upadate_musicUI();

    /* 绑定播放面板展开/折叠按钮 */
    if (objects.full_btn)
        lv_obj_add_event_cb(objects.full_btn, expand_main_panel, LV_EVENT_CLICKED, NULL);
    if (objects.full_btn_1)
        lv_obj_add_event_cb(objects.full_btn_1, collapse_main_panel, LV_EVENT_CLICKED, NULL);

    /* 绑定小组件播放控制按钮 */
    if (objects.pause_btn)
        lv_obj_add_event_cb(objects.pause_btn, play_pause_cb, LV_EVENT_CLICKED, NULL);
    if (objects.forward_btn)
        lv_obj_add_event_cb(objects.forward_btn, next_song_cb, LV_EVENT_CLICKED, NULL);
    if (objects.backword_btn)
        lv_obj_add_event_cb(objects.backword_btn, prev_song_cb, LV_EVENT_CLICKED, NULL);

    /* 绑定全屏播放面板的播放控制按钮 */
    if (objects.pause_main_btn)
        lv_obj_add_event_cb(objects.pause_main_btn, play_pause_cb, LV_EVENT_CLICKED, NULL);
    if (objects.forward_main_btn)
        lv_obj_add_event_cb(objects.forward_main_btn, next_song_cb, LV_EVENT_CLICKED, NULL);
    if (objects.backword_main_btn)
        lv_obj_add_event_cb(objects.backword_main_btn, prev_song_cb, LV_EVENT_CLICKED, NULL);

    /* 绑定歌曲列表按钮（可切换状态） */
    if (objects.songlist_btn) {
        lv_obj_add_flag(objects.songlist_btn, LV_OBJ_FLAG_CHECKABLE);
        lv_obj_add_event_cb(objects.songlist_btn, songlist_btn_cb, LV_EVENT_CLICKED, NULL);
    }
}
