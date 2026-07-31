/**
 * @file    player_ui.h
 * @brief   音乐播放器 UI 模块
 * @details 负责音乐播放控制回调、歌曲列表管理、进度条更新、播放面板展开/折叠动画。
 *          底层音频播放由 music.c 模块实现（mplayer 从模式）。
 * @note    保留了 EEZ Studio 生成的原始对象名（如 backword_btn）。
 */

#ifndef UI_PLAYER_UI_H
#define UI_PLAYER_UI_H

#include <lvgl/lvgl.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief 初始化音乐播放器 UI 模块
 * @details 依次执行：
 *          1. 初始化底层音乐播放器（music_init）
 *          2. 创建进度条更新定时器（每秒刷新）
 *          3. 构建歌曲列表并绑定所有音乐相关事件回调
 *          4. 设置默认 UI 状态（隐藏大面板、同步当前歌曲信息）
 */
void player_ui_init(void);

/* ------------------------------------------------------------------ */
/* 播放控制事件回调                                                      */
/* ------------------------------------------------------------------ */

/** 播放/暂停按钮 — 切换播放状态并刷新 UI */
void play_pause_cb(lv_event_t *e);

/** 下一首按钮 — 切换到下一首歌曲并刷新 UI */
void next_song_cb(lv_event_t *e);

/** 上一首按钮 — 切换到上一首歌曲并刷新 UI */
void prev_song_cb(lv_event_t *e);

/* ------------------------------------------------------------------ */
/* 播放面板展开/折叠                                                     */
/* ------------------------------------------------------------------ */

/** 展开全屏播放面板 — 隐藏小组件面板，滑入大播放面板 */
void expand_main_panel(lv_event_t *e);

/** 收起全屏播放面板 — 滑出大播放面板，滑入小组件面板 */
void collapse_main_panel(lv_event_t *e);

/* ------------------------------------------------------------------ */
/* 歌曲列表                                                             */
/* ------------------------------------------------------------------ */

/** 歌曲列表按钮 — 切换歌曲列表（roller）的显示/隐藏 */
void songlist_btn_cb(lv_event_t *e);

#ifdef __cplusplus
}
#endif

#endif /* UI_PLAYER_UI_H */
