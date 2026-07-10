#include "myhead.h"
#include "ui.h"

static int bg_count = 0;
static int current_bg_index = -1;
static char bg_file[MAX_BGIMG][256];

// 扫描文件夹
static void load_BG()
{
    bg_count = 0;
    current_bg_index = -1;

    mkdir("/userdata/Bg", 0755); // 确保目录存在

    DIR *dir = opendir("/userdata/Bg");
    if (dir != NULL) {
        struct dirent *ent;
        while ((ent = readdir(dir)) != NULL && bg_count < MAX_BGIMG) {
            if (strstr(ent->d_name, ".bmp")) {
                sprintf(bg_file[bg_count], "/userdata/Bg/%s", ent->d_name);
                bg_count++;
            }
        }
        closedir(dir);
    }
}

//亮度
static void set_lcd_brightness(int value)
{
    char *path = "/sys/devices/platform/backlight/backlight/backlight/brightness";
    
    FILE *fp = fopen(path, "w");
    if (fp != NULL) 
    {
        fprintf(fp, "%d", value);
        fclose(fp);
    } 
    else 
    {
        printf("无法打开亮度文件");
    }
}

static void update_BG_preview(void);

// tabview 切换事件
static void tabview_change_event(lv_event_t * e)
{
    uint16_t active_tab = lv_tabview_get_tab_act(ui_SettingTabview);

    if (active_tab == 0) 
    {  
        // Password 标签页
        lv_obj_clear_flag(ui_PsdChangePanel, LV_OBJ_FLAG_HIDDEN);
        lv_obj_add_flag(ui_BGchangePanel, LV_OBJ_FLAG_HIDDEN);
        lv_obj_add_flag(ui_BrighrPanel,LV_OBJ_FLAG_HIDDEN);

        if (current_login_user != NULL) {
            lv_label_set_text_fmt(ui_currentUserlabel, "Current User: %s", current_login_user->username);
        }
    }
    else if (active_tab == 1)
    {
        // Background 标签页
        lv_obj_clear_flag(ui_BGchangePanel, LV_OBJ_FLAG_HIDDEN);
        lv_obj_add_flag(ui_PsdChangePanel, LV_OBJ_FLAG_HIDDEN);
        lv_obj_add_flag(ui_BrighrPanel,LV_OBJ_FLAG_HIDDEN);

        load_BG();

        if (bg_count == 0) {
            lv_roller_set_options(ui_BGroller, "Empty", LV_ROLLER_MODE_NORMAL);
            lv_img_set_src(ui_BgPreview, NULL);
            return;
        }

        char option_str[1024] = {0};
        for (int i = 0; i < bg_count; i++) {
            char * filename = strrchr(bg_file[i], '/');
            if (filename != NULL) 
            {
                filename += 1;
            }
            else
            {
                filename = bg_file[i];
            }    
            strcat(option_str, filename);
            if (i < bg_count - 1) 
            {
                strcat(option_str, "\n");
            }
        }
        lv_roller_set_options(ui_BGroller, option_str, LV_ROLLER_MODE_NORMAL);

        current_bg_index = 0;
        lv_roller_set_selected(ui_BGroller, 0, LV_ANIM_OFF);
        update_BG_preview();
    }
    else if (active_tab == 2)
    {
        lv_obj_add_flag(ui_BGchangePanel, LV_OBJ_FLAG_HIDDEN);
        lv_obj_add_flag(ui_PsdChangePanel, LV_OBJ_FLAG_HIDDEN);
        lv_obj_clear_flag(ui_BrighrPanel,LV_OBJ_FLAG_HIDDEN);
        
    }
    
    else if (active_tab == 3)
    {
        _ui_screen_change(&ui_MainPage, LV_SCR_LOAD_ANIM_FADE_ON, 500, 0, &ui_MainPage_screen_init);
    }
    
}

// 刷新右侧预览图
static void update_BG_preview(void)
{
    if (bg_count == 0 || current_bg_index < 0) return;

    static char lvgl_path[300];
    sprintf(lvgl_path, "S:%s", bg_file[current_bg_index]);
    lv_img_cache_invalidate_src(NULL);
    lv_img_set_src(ui_BgPreview, lvgl_path);
}

// 滚轮滑动
static void bg_roller_change(lv_event_t * e)
{
    current_bg_index = lv_roller_get_selected(ui_BGroller);
    update_BG_preview();
}

//亮度滑动事件
static void brightness_slider_event(lv_event_t * e)
{
    int value = lv_slider_get_value(ui_BrightSlider);

    set_lcd_brightness(value);
}

//背景确认
static void confirm_bg_btn_event(lv_event_t * e)
{
    if (bg_count == 0 || current_bg_index < 0) return;

    static char lvgl_path[300];
    sprintf(lvgl_path, "S:%s", bg_file[current_bg_index]);
    lv_img_cache_invalidate_src(NULL);
    lv_img_set_src(ui_MainBg, lvgl_path); 
}

// 密码修改执行按钮
static void Psd_Change_event(lv_event_t * e)
{
    if (current_login_user == NULL) return;

    const char * new_psd1 = lv_textarea_get_text(ui_PsdChange1);
    const char * new_psd2 = lv_textarea_get_text(ui_PsdChange2);

    if (strlen(new_psd1) == 0 || strlen(new_psd2) == 0) {
        lv_label_set_text(ui_warrningLabel, "Password cannot be empty!");
        lv_obj_set_style_text_color(ui_warrningLabel, lv_color_hex(0xFF0000), 0);
        return;
    }
    if (strcmp(new_psd1, new_psd2) != 0) {
        lv_label_set_text(ui_warrningLabel, "Two password do not match!");
        lv_obj_set_style_text_color(ui_warrningLabel, lv_color_hex(0xFF0000), 0);
        return;
    }

    strcpy(current_login_user->password, new_psd1);

    FILE *fp = fopen(USER_DATA_FILE, "w");
    if (!fp) return;
    struct user * pos;
    list_for_each_entry(pos, &gloabl_user_list, list) {
        fprintf(fp, "%s,%s,%d\n", pos->username, pos->password, pos->role);
    }
    fclose(fp);

    lv_label_set_text(ui_warrningLabel, "Password changed sucessfuly!");
    lv_obj_set_style_text_color(ui_warrningLabel, lv_color_hex(0x00FF00), 0);
    lv_textarea_set_text(ui_PsdChange1, "");
    lv_textarea_set_text(ui_PsdChange2, "");
}

void Seeting_page_init(void)
{
    // 密码功能绑定
    lv_obj_add_event_cb(ui_PsdChangeconfirmBtn, Psd_Change_event, LV_EVENT_CLICKED, NULL);
    
    // 壁纸功能绑定
    lv_obj_add_event_cb(ui_BGroller, bg_roller_change, LV_EVENT_VALUE_CHANGED, NULL);
    lv_obj_add_event_cb(ui_BGconfirmBtn, confirm_bg_btn_event, LV_EVENT_CLICKED, NULL);

    // tabview 标签页切换（监听 tabview 本体而非按钮）
    lv_obj_add_event_cb(ui_SettingTabview, tabview_change_event, LV_EVENT_VALUE_CHANGED, NULL);

    //亮度滑条
    lv_obj_add_event_cb(ui_BrightSlider,brightness_slider_event,LV_EVENT_VALUE_CHANGED,NULL);

    //初始设置滑条范围
    lv_slider_set_range(ui_BrightSlider,0,255);
    lv_slider_set_value(ui_BrightSlider,200,LV_ANIM_OFF);
}