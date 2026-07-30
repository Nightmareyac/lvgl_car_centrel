#include "actions.h"
#include "action1.h"
#include "ui.h"
#include "music.h"
#include "timePanel.h"
#include "lvgl/lvgl.h"
#include <pthread.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <time.h>
#include "bemfa_client.h"

// 映射巴法云的主题 ID
#define TOPIC_TRUNK "mTJT3Afrk006"  // 后备箱
#define TOPIC_FRUNK "vc8lsGpsp006"  // 引擎盖
#define TOPIC_LOCK "DyQL1Jcsc004"   // 车锁
#define TOPIC_AC "61SOh2Izr005"     // 空调
#define TOPIC_LIGHT1 "4lx3WVhUA002" // 灯1
#define TOPIC_LIGHT2 "6MHL9hbRf002" // 灯2
#define TOPIC_LIGHT3 "EIHBf8Q8A002" // 灯3

static void upadate_musicUI(void); // 前向声明
static void slide_in_panel(lv_obj_t * panel, int x_target, int duration_ms);
static void slide_out_panel(lv_obj_t * panel, int x_start, int duration_ms, lv_anim_ready_cb_t ready_cb);


void sound_bar_cb(lv_event_t *e)
{
    if (!objects.sound_bar)
    {
        return;
    }
    int value = lv_slider_get_value(objects.sound_bar);
    music_set_volume(value);

    printf("[sounde volume] set value to:%d \n",value);
    
}

static void sound_btn_cb(lv_event_t *e)
{
    if (lv_obj_has_state(objects.sound_btn,LV_STATE_CHECKED))
    {
        lv_obj_clear_flag(objects.sound_bar,LV_OBJ_FLAG_HIDDEN);
    }
    else
    {
        lv_obj_add_flag(objects.sound_bar,LV_OBJ_FLAG_HIDDEN);
    }   
}

void bemfa_ui_update_cb(lv_timer_t * timer)
{
    if(!g_bemfa_state.is_updated) {
        return;
    }

    if(objects.trunk_btn) { // 后备箱
        if(g_bemfa_state.trunk_open)
            lv_obj_add_state(objects.trunk_btn, LV_STATE_CHECKED);
        else
            lv_obj_clear_state(objects.trunk_btn, LV_STATE_CHECKED);
    }

    if(objects.lock_btn) { // 锁
        if(g_bemfa_state.lock_locked)
            lv_obj_add_state(objects.lock_btn, LV_STATE_CHECKED);
        else
            lv_obj_clear_state(objects.lock_btn, LV_STATE_CHECKED);
    }

    if(objects.airconditionr_call_btn) { // 空调
        if(g_bemfa_state.ac_on)
            lv_obj_add_state(objects.airconditionr_call_btn, LV_STATE_CHECKED);
        else
            lv_obj_clear_state(objects.airconditionr_call_btn, LV_STATE_CHECKED);
    }

    if(objects.funk_btn) { // 引擎盖
        if(g_bemfa_state.frunk_open)
            lv_obj_add_state(objects.funk_btn, LV_STATE_CHECKED);
        else
            lv_obj_clear_state(objects.funk_btn, LV_STATE_CHECKED);
    }

    if(objects.flash1) { // 灯1
        if(g_bemfa_state.flash1_turn)
            lv_obj_add_state(objects.flash1, LV_STATE_CHECKED);
        else
            lv_obj_clear_state(objects.flash1, LV_STATE_CHECKED);
    }

    if(objects.flash2) { // 灯2
        if(g_bemfa_state.flash2_turn)
            lv_obj_add_state(objects.flash2, LV_STATE_CHECKED);
        else
            lv_obj_clear_state(objects.flash2, LV_STATE_CHECKED);
    }

    if(objects.flash3) { // 灯3
        if(g_bemfa_state.flash3_turn)
            lv_obj_add_state(objects.flash3, LV_STATE_CHECKED);
        else
            lv_obj_clear_state(objects.flash3, LV_STATE_CHECKED);
    }

    // 标记信息已处理
    g_bemfa_state.is_updated = false;
}

static void set_brighness(int value)
{
    char *path = "/sys/devices/platform/backlight/backlight/backlight/brightness";

    FILE *fp = fopen(path,"w");
    if (fp != NULL)
    {
        fprintf(fp,"%d",value);
        fclose(fp);
    }
    else
    {
        printf("cannot open brightness file");
    }
    
}

void brightness_slider_cb(lv_event_t *e)
{
    if (!objects.brightness_bar)
    {
        return;
    }
    int value = lv_slider_get_value(objects.brightness_bar);
    set_brighness(value);

    printf("[brightness] set value to:%d \n",value);
    
}

static void brightness_btn_cb(lv_event_t *e)
{
    if (lv_obj_has_state(objects.brightness_btn,LV_STATE_CHECKED))
    {
        lv_obj_clear_flag(objects.brightness_bar,LV_OBJ_FLAG_HIDDEN);
    }
    else
    {
        lv_obj_add_flag(objects.brightness_bar,LV_OBJ_FLAG_HIDDEN);
    }
    
}

// 1. 后备箱按键回调
void action_trunk_btn_click(lv_event_t * e)
{
    static bool open_state = false;
    open_state             = !open_state;

    // 向巴法云发送 on / off 或 open / close
    bemfa_send_msg(TOPIC_TRUNK, open_state ? "on" : "off");
}

// 2. 引擎盖按键回调
void action_frunk_btn_click(lv_event_t * e)
{
    static bool open_state = false;
    open_state             = !open_state;

    bemfa_send_msg(TOPIC_FRUNK, open_state ? "on" : "off");
}

// 3. 车锁按键回调
void action_lock_btn_click(lv_event_t * e)
{
    static bool is_locked = false;
    is_locked             = !is_locked;

    bemfa_send_msg(TOPIC_LOCK, is_locked ? "lock" : "unlock");
}

// 灯1按键回调
void action_flash1_btn_click(lv_event_t * e)
{
    static bool light_on = false;
    light_on             = !light_on;

    bemfa_send_msg(TOPIC_LIGHT1, light_on ? "on" : "off");
}

// 灯2按键回调
void action_flash2_btn_click(lv_event_t * e)
{
    static bool light_on = false;
    light_on             = !light_on;

    bemfa_send_msg(TOPIC_LIGHT2, light_on ? "on" : "off");
}
// 灯3按键回调
void action_flash3_btn_click(lv_event_t * e)
{
    static bool light_on = false;
    light_on             = !light_on;

    bemfa_send_msg(TOPIC_LIGHT3, light_on ? "on" : "off");
}

// 空调触发回调
void action_airconditionr_call_btn_click(lv_event_t * e)
{
    if(!objects.tem_roller) return;

    lv_obj_t * btn = lv_event_get_target(e);

    if(lv_obj_has_state(btn, LV_STATE_CHECKED)) {
        // 显示温度滚轮
        lv_obj_clear_flag(objects.tem_roller, LV_OBJ_FLAG_HIDDEN);
    } else {
        // 隐藏滚轮并读取选中温度
        lv_obj_add_flag(objects.tem_roller, LV_OBJ_FLAG_HIDDEN);

        // 获取 roller 选中的索引
        uint16_t selected = lv_roller_get_selected(objects.tem_roller);

        // 获取对应的温度文本（假设 roller 选项格式为 "16°C\n17°C\n18°C..."）
        char temp_buf[16];
        lv_roller_get_selected_str(objects.tem_roller, temp_buf, sizeof(temp_buf));

        // 发送给巴法云（格式：setTemp=25 或直接发温度值）
        bemfa_send_msg(TOPIC_AC, temp_buf);

        printf("[AC] Temperature set to: %s (index=%d)\n", temp_buf, selected);
    }
}

// 时间更新
void update_time()
{
    time_t now;
    struct tm tm;

    now = time(NULL);
    localtime_r(&now, &tm);

    lv_label_set_text_fmt(objects.time_label, "%02d:%02d", tm.tm_hour, tm.tm_min);
    lv_label_set_text_fmt(objects.year_label, "%04d/%02d/%02d", tm.tm_year + 1900, tm.tm_mon, tm.tm_mday);
}

// 天气面板收起动画结束回调
static void wealth_collapse_ready_cb(lv_anim_t * a)
{
    lv_obj_add_flag(objects.wealth_panel, LV_OBJ_FLAG_HIDDEN);
}

// 天气btn事件
void wealth_btn_cb(lv_event_t * e)
{
    if(!objects.wealth_panel) return;
    lv_obj_t * btn = lv_event_get_target(e);
    if(lv_obj_has_state(btn, LV_STATE_CHECKED)) {
        slide_in_panel(objects.wealth_panel, 540, 350);
        // 后台线程拉取天气数据
        pthread_t thread_id;
        pthread_create(&thread_id, NULL, weather_thread_func, NULL);
        pthread_detach(thread_id);
    } else {
        slide_out_panel(objects.wealth_panel, 540, 300, wealth_collapse_ready_cb);
    }
}

// 日历事件
void calander_btn_cb(lv_event_t * e)
{
    if(!objects.calander) return;
    lv_obj_t * btn = lv_event_get_target(e);
    if(lv_obj_has_state(btn, LV_STATE_CHECKED)) {
        lv_obj_clear_flag(objects.calander, LV_OBJ_FLAG_HIDDEN);
    } else {
        lv_obj_add_flag(objects.calander, LV_OBJ_FLAG_HIDDEN);
    }
}

// roller选择事件
static void songlist_item_cb(lv_event_t * e)
{
    lv_obj_t * roller = lv_event_get_target(e);
    uint16_t index    = lv_roller_get_selected(roller);
    music_jump_to(index);
    upadate_musicUI();

    lv_obj_add_flag(objects.songlist, LV_OBJ_FLAG_HIDDEN);
    lv_obj_clear_state(objects.songlist_btn, LV_STATE_CHECKED);
}

// 构建roller歌单选项
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

// 歌曲roller事件
void songlist_btn_cb(lv_event_t * e)
{
    if(!objects.songlist) {
        printf("[ERROR] objects.songlist is NULL\n");
        return;
    }
    lv_obj_t * btn = lv_event_get_target(e);

    if(lv_obj_has_state(btn, LV_STATE_CHECKED)) {
        lv_obj_clear_flag(objects.songlist, LV_OBJ_FLAG_HIDDEN);
    } else {
        lv_obj_add_flag(objects.songlist, LV_OBJ_FLAG_HIDDEN);
    }
}

// 收起动画结束回调，隐藏mainpanel
static void collapes_ready_cb(lv_anim_t * a)
{
    lv_obj_add_flag(objects.player_main_panel, LV_OBJ_FLAG_HIDDEN);
    lv_obj_add_flag(objects.songlist, LV_OBJ_FLAG_HIDDEN);
}

// 更新音乐ui
static void upadate_musicUI(void)
{
    Song * cur = music_get_current_song();
    if(!cur) {
        return;
    }

    // 组件歌名
    if(objects.song_name) {
        lv_label_set_text(objects.song_name, cur->tittle);
    }
    // 主页面歌名
    if(objects.song_tittle) {
        lv_label_set_text(objects.song_tittle, cur->tittle);
    }

    // 歌手
    if(objects.songer) {
        lv_label_set_text(objects.songer, cur->songer);
    }

    // 主背景
    if(objects.song_panel_main && cur->cover_path) {
        lv_obj_set_style_bg_img_src(objects.song_panel_main, cur->cover_path1, LV_PART_MAIN | LV_STATE_DEFAULT);
    }

    // 封面
    if(objects.song_panel && cur->cover_path) {
        lv_obj_set_style_bg_img_src(objects.song_panel, cur->cover_path, LV_PART_MAIN | LV_STATE_DEFAULT);
    }

    // 进度条
    if(objects.progress_bar) {
        lv_bar_set_value(objects.progress_bar, 0, LV_ANIM_OFF);
    }

    // 同步两个pause按钮状态（播放中=checked，暂停/停止=unchecked）
    if(music_is_playing()) {
        if(objects.pause_btn) lv_obj_add_state(objects.pause_btn, LV_STATE_CHECKED);
        if(objects.pause_main_btn) lv_obj_add_state(objects.pause_main_btn, LV_STATE_CHECKED);
    } else {
        if(objects.pause_btn) lv_obj_clear_state(objects.pause_btn, LV_STATE_CHECKED);
        if(objects.pause_main_btn) lv_obj_clear_state(objects.pause_main_btn, LV_STATE_CHECKED);
    }
}

// 按钮回调
void play_pause_cb(lv_event_t * e)
{
    music_play_toggle();
    upadate_musicUI();
}

// 下一首
void next_song_cb(lv_event_t * e)
{
    music_next();
    upadate_musicUI();
}

// 上一首
void prev_song_cb(lv_event_t * e)
{
    music_prev();
    upadate_musicUI();
}

// 通用滑入动画（从左侧 -300 滑到目标位置）
// panel: 要动画的面板对象
// x_target: 目标 x 坐标
// duration_ms: 动画时长（毫秒）
static void slide_in_panel(lv_obj_t * panel, int x_target, int duration_ms)
{
    if(!panel) return;
    lv_obj_clear_flag(panel, LV_OBJ_FLAG_HIDDEN);
    lv_obj_set_x(panel, -300);

    lv_anim_t a;
    lv_anim_init(&a);
    lv_anim_set_var(&a, panel);
    lv_anim_set_values(&a, -300, x_target);
    lv_anim_set_time(&a, duration_ms);
    lv_anim_set_path_cb(&a, lv_anim_path_ease_in_out);
    lv_anim_set_exec_cb(&a, (lv_anim_exec_xcb_t)lv_obj_set_x);
    lv_anim_start(&a);
}

// 通用滑出动画（从起始位置滑到 -300）
// panel: 要动画的面板对象
// x_start: 起始 x 坐标
// duration_ms: 动画时长（毫秒）
// ready_cb: 动画结束后的回调函数（可选，传 NULL 则无回调）
static void slide_out_panel(lv_obj_t * panel, int x_start, int duration_ms, lv_anim_ready_cb_t ready_cb)
{
    if(!panel) return;
    lv_anim_t a;
    lv_anim_init(&a);
    lv_anim_set_var(&a, panel);
    lv_anim_set_values(&a, x_start, -300);
    lv_anim_set_time(&a, duration_ms);
    lv_anim_set_path_cb(&a, lv_anim_path_ease_in);
    lv_anim_set_exec_cb(&a, (lv_anim_exec_xcb_t)lv_obj_set_x);
    if(ready_cb) lv_anim_set_ready_cb(&a, ready_cb);
    lv_anim_start(&a);
}

// 音乐全屏展开事件处理
void expand_main_panel(lv_event_t * e)
{
    lv_obj_add_flag(objects.player_widget_panel, LV_OBJ_FLAG_HIDDEN);
    slide_in_panel(objects.player_main_panel, 17, 350);
}

// 音乐全屏收起事件
void collapse_main_panel(lv_event_t * e)
{
    slide_out_panel(objects.player_main_panel, 17, 300, collapes_ready_cb);
    slide_in_panel(objects.player_widget_panel, 17, 300);
}

// 进度条定时更新（每秒）
static void progress_timer_cb(lv_timer_t * t)
{
    if(objects.progress_bar && music_is_playing()) {
        lv_bar_set_value(objects.progress_bar, music_get_progress(), LV_ANIM_OFF);
    }
}

// 手动绑定事件与默认状态初始化
void player_events_init(void)
{ // 初始化音频数据
    music_init();

    // 进度条定时器，每秒刷新一次
    lv_timer_create(progress_timer_cb, 1000, NULL);

    // 创建巴法云 UI 同步定时器，每 100ms 检查一次状态变动
    lv_timer_create(bemfa_ui_update_cb, 100, NULL);

    if (objects.sound_bar)
    {
        // 1. 默认隐藏音量条
        lv_obj_add_flag(objects.sound_bar, LV_OBJ_FLAG_HIDDEN);

        // 2. 设置 Slider 范围为 0 ~ 100
        lv_slider_set_range(objects.sound_bar, 0, 100);

        // 3. 绑定 Slider 变化事件
        lv_obj_add_event_cb(objects.sound_bar, sound_bar_cb, LV_EVENT_VALUE_CHANGED, NULL);
    }

    // 默认隐藏大面板
    if(objects.player_main_panel) {
        lv_obj_add_flag(objects.player_main_panel, LV_OBJ_FLAG_HIDDEN);
    }

    // 默认隐藏天气面板和日历
    if(objects.wealth_panel) {
        lv_obj_add_flag(objects.wealth_panel, LV_OBJ_FLAG_HIDDEN);
    }
    if(objects.calander) {
        lv_obj_add_flag(objects.calander, LV_OBJ_FLAG_HIDDEN);
    }
    // // 默认隐藏温度滚轮
    // if(objects.tem_roller) {
    //     lv_obj_add_flag(objects.tem_roller, LV_OBJ_FLAG_HIDDEN);
    // }

    //亮度调整
    if (objects.brightness_bar)
    {
        lv_obj_add_flag(objects.brightness_bar, LV_OBJ_FLAG_HIDDEN);
        lv_obj_add_event_cb(objects.brightness_bar,brightness_slider_cb,LV_EVENT_VALUE_CHANGED,NULL);
    }
    if (objects.brightness_btn) {
        lv_obj_add_event_cb(objects.brightness_btn, brightness_btn_cb, LV_EVENT_CLICKED, NULL);
    }
    
    if (objects.sound_bar)
    {
        lv_obj_add_flag(objects.sound_bar, LV_OBJ_FLAG_HIDDEN);
        lv_slider_set_range(objects.sound_bar, 0, 100);
        lv_obj_add_event_cb(objects.sound_bar,sound_bar_cb,LV_EVENT_VALUE_CHANGED,NULL);
    }
    if (objects.sound_btn)
    {
        lv_obj_add_flag(objects.sound_btn, LV_OBJ_FLAG_CHECKABLE);
        lv_obj_add_event_cb(objects.sound_btn,sound_btn_cb,LV_EVENT_CLICKED,NULL);
    }
    


    // wealth_btn / calander_btn / airconditionr_call_btn
    if(objects.wealth_btn) {
        lv_obj_add_flag(objects.wealth_btn, LV_OBJ_FLAG_CHECKABLE);
        lv_obj_add_event_cb(objects.wealth_btn, wealth_btn_cb, LV_EVENT_CLICKED, NULL);
    }
    if(objects.calander_btn) {
        lv_obj_add_flag(objects.calander_btn, LV_OBJ_FLAG_CHECKABLE);
        lv_obj_add_event_cb(objects.calander_btn, calander_btn_cb, LV_EVENT_CLICKED, NULL);
    }
    if(objects.airconditionr_call_btn) {
        lv_obj_add_flag(objects.airconditionr_call_btn, LV_OBJ_FLAG_CHECKABLE);
        lv_obj_add_event_cb(objects.airconditionr_call_btn, action_airconditionr_call_btn_click, LV_EVENT_CLICKED,
                            NULL);
    }

    // 车辆控制按钮（后备箱/引擎盖/车锁/车灯）
    if(objects.trunk_btn) lv_obj_add_event_cb(objects.trunk_btn, action_trunk_btn_click, LV_EVENT_CLICKED, NULL);
    if(objects.funk_btn) lv_obj_add_event_cb(objects.funk_btn, action_frunk_btn_click, LV_EVENT_CLICKED, NULL);
    if(objects.lock_btn) lv_obj_add_event_cb(objects.lock_btn, action_lock_btn_click, LV_EVENT_CLICKED, NULL);
    if(objects.flash1) lv_obj_add_event_cb(objects.flash1, action_flash1_btn_click, LV_EVENT_CLICKED, NULL);
    if(objects.flash2) lv_obj_add_event_cb(objects.flash2, action_flash2_btn_click, LV_EVENT_CLICKED, NULL);
    if(objects.flash3) lv_obj_add_event_cb(objects.flash3, action_flash3_btn_click, LV_EVENT_CLICKED, NULL);

    // 初始化songlist roller选项并绑定事件
    if(objects.songlist) {
        build_songlist(objects.songlist);
        lv_obj_add_flag(objects.songlist, LV_OBJ_FLAG_HIDDEN);
        lv_obj_add_event_cb(objects.songlist, songlist_item_cb, LV_EVENT_VALUE_CHANGED, NULL);
    }

    // 初始化ui
    upadate_musicUI();

    // 按钮事件绑定
    if(objects.full_btn) lv_obj_add_event_cb(objects.full_btn, expand_main_panel, LV_EVENT_CLICKED, NULL);
    if(objects.full_btn_1) lv_obj_add_event_cb(objects.full_btn_1, collapse_main_panel, LV_EVENT_CLICKED, NULL);

    if(objects.pause_btn) lv_obj_add_event_cb(objects.pause_btn, play_pause_cb, LV_EVENT_CLICKED, NULL);
    if(objects.forward_btn) lv_obj_add_event_cb(objects.forward_btn, next_song_cb, LV_EVENT_CLICKED, NULL);
    if(objects.backword_btn) lv_obj_add_event_cb(objects.backword_btn, prev_song_cb, LV_EVENT_CLICKED, NULL);

    if(objects.pause_main_btn) lv_obj_add_event_cb(objects.pause_main_btn, play_pause_cb, LV_EVENT_CLICKED, NULL);
    if(objects.forward_main_btn) lv_obj_add_event_cb(objects.forward_main_btn, next_song_cb, LV_EVENT_CLICKED, NULL);
    if(objects.backword_main_btn) lv_obj_add_event_cb(objects.backword_main_btn, prev_song_cb, LV_EVENT_CLICKED, NULL);

    // songlist按钮
    if(objects.songlist_btn) {
        lv_obj_add_flag(objects.songlist_btn, LV_OBJ_FLAG_CHECKABLE);
        lv_obj_add_event_cb(objects.songlist_btn, songlist_btn_cb, LV_EVENT_CLICKED, NULL);
    }

    // 进度条定时器，每秒刷新一次
    lv_timer_create(progress_timer_cb, 1000, NULL);
}