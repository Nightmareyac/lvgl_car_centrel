#include "player_ui.h"
#include "screens.h"
#include "music.h"
#include "anim.h"

#include <stdio.h>
#include <string.h>

// 内部辅助函数声明

static void upadate_musicUI(void);
static void build_songlist(lv_obj_t * roller);
static void mount_music_overlay(void);
static void mount_vehicle_controls(void);

//置顶播放器
static void mount_music_overlay(void)
{
    if(!objects.music_all_panel) return;

    lv_obj_set_parent(objects.music_all_panel, lv_layer_top());
    lv_obj_set_pos(objects.music_all_panel, 0, 0);
    lv_obj_set_size(objects.music_all_panel, lv_disp_get_hor_res(NULL), lv_disp_get_ver_res(NULL));
    lv_obj_clear_flag(objects.music_all_panel, LV_OBJ_FLAG_HIDDEN | LV_OBJ_FLAG_CLICKABLE | LV_OBJ_FLAG_SCROLLABLE);
    lv_obj_move_foreground(objects.music_all_panel);
}

//车灯复用
static void mount_vehicle_controls(void)
{
    static const struct
    {
        lv_obj_t ** object;
        lv_coord_t x;
        lv_coord_t y;
    } controls[] = {
        {&objects.flash1, 38, 130},
        {&objects.flash2, 38, 209},
        {&objects.flash3, 38, 289},
        {&objects.flash4, 38, 379},
    };

    if(!objects.music_all_panel) return;

    for(int i = 0; i < sizeof(controls) / sizeof(controls[0]); i++) {
        if(!*controls[i].object) continue;

        // 绑定父容器，实现控件复用
        if(lv_obj_get_parent(*controls[i].object) != objects.music_all_panel)
            lv_obj_set_parent(*controls[i].object, objects.music_all_panel);

        lv_obj_set_pos(*controls[i].object, controls[i].x, controls[i].y);
    }
}

// 内部回调函数                                                          */
// 歌曲列表 roller 选择事件回调
static void songlist_item_cb(lv_event_t * e)
{
    lv_obj_t * roller = lv_event_get_target(e);
    uint16_t index    = lv_roller_get_selected(roller);

    music_jump_to(index);
    upadate_musicUI();

    /* 选择后隐藏列表并取消按钮选中状态 */
    lv_obj_add_flag(objects.songlist, LV_OBJ_FLAG_HIDDEN);
    lv_obj_clear_state(objects.songlist_btn, LV_STATE_CHECKED);
}

// 收起动画结束回调,隐藏mianplayer
static void collapes_ready_cb(lv_anim_t * a)
{
    lv_obj_add_flag(objects.player_main_panel, LV_OBJ_FLAG_HIDDEN);
    lv_obj_add_flag(objects.songlist, LV_OBJ_FLAG_HIDDEN);
}

// 进度条回调（每秒执行一次）
static void progress_timer_cb(lv_timer_t * t)
{
    if(objects.progress_bar && music_is_playing()) {
        lv_bar_set_value(objects.progress_bar, music_get_progress(), LV_ANIM_OFF);
    }
}

// 播放控制回调                                                          */
// 暂停按钮回调切换播放状态并刷新 UI
void play_pause_cb(lv_event_t * e)
{
    music_play_toggle();
    upadate_musicUI();
}

// 下一首按钮回调,切换到下一首歌曲并刷新 UI
void next_song_cb(lv_event_t * e)
{
    music_next();
    upadate_musicUI();
}

// 上一首按钮回调 — 切换到上一首歌曲并刷新 UI
void prev_song_cb(lv_event_t * e)
{
    music_prev();
    upadate_musicUI();
}

// 播放面板展开/折叠                                                     */
// 展开全屏播放面板
void expand_main_panel(lv_event_t * e)
{
    lv_obj_add_flag(objects.player_widget_panel, LV_OBJ_FLAG_HIDDEN);
    slide_in_panel(objects.player_main_panel, 17, 350);
}

// 收起全屏播放面板
void collapse_main_panel(lv_event_t * e)
{
    slide_out_panel(objects.player_main_panel, 17, 300, collapes_ready_cb);
    slide_in_panel(objects.player_widget_panel, 17, 300);
}

// 歌曲列表
void songlist_btn_cb(lv_event_t * e)
{
    if(!objects.songlist) {
        printf("songlist is NULL\n");
        return;
    }

    lv_obj_t * btn = lv_event_get_target(e);
    if(lv_obj_has_state(btn, LV_STATE_CHECKED)) {
        lv_obj_clear_flag(objects.songlist, LV_OBJ_FLAG_HIDDEN);
    } else {
        lv_obj_add_flag(objects.songlist, LV_OBJ_FLAG_HIDDEN);
    }
}

// 内部辅助函数实现
// 更新音乐 UI 显示
static void upadate_musicUI(void)
{
    Song * cur = music_get_current_song();
    if(!cur) return;

    // 更新歌名（widget + main 同步)
    if(objects.song_name) lv_label_set_text(objects.song_name, cur->tittle);
    if(objects.song_tittle) lv_label_set_text(objects.song_tittle, cur->tittle);

    // 更新歌手
    if(objects.songer) lv_label_set_text(objects.songer, cur->songer);

    // 更新封面（主背景用 cover_path1，小组件用 cover_path)
    if(objects.song_panel_main && cur->cover_path1) {
        lv_obj_set_style_bg_img_src(objects.song_panel_main, cur->cover_path1, LV_PART_MAIN | LV_STATE_DEFAULT);
    }
    if(objects.song_panel && cur->cover_path) {
        lv_obj_set_style_bg_img_src(objects.song_panel, cur->cover_path, LV_PART_MAIN | LV_STATE_DEFAULT);
    }

    // 进度条归零（切歌后重新开始)
    if(objects.progress_bar) lv_bar_set_value(objects.progress_bar, 0, LV_ANIM_OFF);

    // 同步两个播放/暂停按钮的选中状态
    if(music_is_playing()) {
        if(objects.pause_btn) lv_obj_add_state(objects.pause_btn, LV_STATE_CHECKED);
        if(objects.pause_main_btn) lv_obj_add_state(objects.pause_main_btn, LV_STATE_CHECKED);
    } else {
        if(objects.pause_btn) lv_obj_clear_state(objects.pause_btn, LV_STATE_CHECKED);
        if(objects.pause_main_btn) lv_obj_clear_state(objects.pause_main_btn, LV_STATE_CHECKED);
    }
}

// 将所有歌曲名用换行符拼接成一个字符串，设置为 roller 的选项
static void build_songlist(lv_obj_t * roller)
{
    if(!roller) return;

    char options[512] = "";
    for(int i = 0; i < song_count; i++) {
        if(i > 0) strcat(options, "\n");
        strcat(options, song_list[i].tittle);
    }
    lv_roller_set_options(roller, options, LV_ROLLER_MODE_NORMAL);
}

// 模块初始化
// 初始化音乐播放器 UI 模块
void player_ui_init(void)
{
    //保持都在顶层页面，实现组件复用
    mount_music_overlay();
    mount_vehicle_controls();

    /* 初始化底层音乐播放器（mplayer 从模式） */
    music_init();

    /* 创建进度条更新定时器（每秒刷新一次） */
    lv_timer_create(progress_timer_cb, 1000, NULL);

    /* 默认隐藏全屏播放面板 */
    if(objects.player_main_panel) lv_obj_add_flag(objects.player_main_panel, LV_OBJ_FLAG_HIDDEN);

    /* 构建歌曲列表并绑定 roller 事件 */
    if(objects.songlist) {
        build_songlist(objects.songlist);
        lv_obj_add_flag(objects.songlist, LV_OBJ_FLAG_HIDDEN);
        lv_obj_add_event_cb(objects.songlist, songlist_item_cb, LV_EVENT_VALUE_CHANGED, NULL);
    }

    /* 初始化当前歌曲 UI 显示 */
    upadate_musicUI();

    /* 绑定播放面板展开/折叠按钮 */
    if(objects.full_btn) lv_obj_add_event_cb(objects.full_btn, expand_main_panel, LV_EVENT_CLICKED, NULL);
    if(objects.full_btn_1) lv_obj_add_event_cb(objects.full_btn_1, collapse_main_panel, LV_EVENT_CLICKED, NULL);

    /* 绑定小组件播放控制按钮 */
    if(objects.pause_btn) lv_obj_add_event_cb(objects.pause_btn, play_pause_cb, LV_EVENT_CLICKED, NULL);
    if(objects.forward_btn) lv_obj_add_event_cb(objects.forward_btn, next_song_cb, LV_EVENT_CLICKED, NULL);
    if(objects.backword_btn) lv_obj_add_event_cb(objects.backword_btn, prev_song_cb, LV_EVENT_CLICKED, NULL);

    /* 绑定全屏播放面板的播放控制按钮 */
    if(objects.pause_main_btn) lv_obj_add_event_cb(objects.pause_main_btn, play_pause_cb, LV_EVENT_CLICKED, NULL);
    if(objects.forward_main_btn) lv_obj_add_event_cb(objects.forward_main_btn, next_song_cb, LV_EVENT_CLICKED, NULL);
    if(objects.backword_main_btn) lv_obj_add_event_cb(objects.backword_main_btn, prev_song_cb, LV_EVENT_CLICKED, NULL);

    /* 绑定歌曲列表按钮（可切换状态） */
    if(objects.songlist_btn) {
        lv_obj_add_flag(objects.songlist_btn, LV_OBJ_FLAG_CHECKABLE);
        lv_obj_add_event_cb(objects.songlist_btn, songlist_btn_cb, LV_EVENT_CLICKED, NULL);
    }
}
