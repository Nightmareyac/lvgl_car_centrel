#include "myhead.h"
#include "ui.h"

extern struct list_head gloabl_user_list;


// 消息框隐藏函数
static void hide_mess_panel(lv_timer_t * t)
{
    lv_obj_add_flag(ui_loginmessPanel, LV_OBJ_FLAG_HIDDEN);
    lv_timer_del(t);
}

// 初始用户加载
void User_Loading(void)
{
    FILE *fp = fopen(USER_DATA_FILE, "r");

    // 如果文件不存在，默认创建一个admin
    if (!fp)
    {
        printf("文件打开失败，创建默认 admin/admin\n");
        struct user *admin = malloc(sizeof(struct user));
        strcpy(admin->username, "admin");
        strcpy(admin->password, "admin");
        admin->role = 1;
        list_add_tail(&admin->list, &gloabl_user_list);
        return;
    }

    char buf[128];
    while (fgets(buf, sizeof(buf), fp))
    {
        char user_name[20], user_pswd[20];
        int user_role = 0;
        if (sscanf(buf, "%[^,],%[^,],%d", user_name, user_pswd, &user_role) == 3)
        {
            struct user *newuser = malloc(sizeof(struct user));
            strcpy(newuser->username, user_name);
            strcpy(newuser->password, user_pswd);
            newuser->role = user_role;

            list_add_tail(&newuser->list, &gloabl_user_list);
        }
    }
    fclose(fp);
}

// 注册逻辑
void register_click(lv_event_t *e)
{
    const char * acc_text = lv_textarea_get_text(ui_Raccount);
    const char * psd_text = lv_textarea_get_text(ui_Rpassword);
    const char * Rpsd_text = lv_textarea_get_text(ui_RpeatePassword);

    // 检查长度
    if (strlen(acc_text) == 0 || strlen(psd_text) == 0 || strlen(Rpsd_text) == 0)
    {
        lv_label_set_text_fmt(ui_loginmessLabel, "Prohibi NULL value");
        lv_obj_clear_flag(ui_loginmessPanel, LV_OBJ_FLAG_HIDDEN);
        lv_timer_create(hide_mess_panel, 1000, NULL);
        return;
    }

    if (strcmp(psd_text, Rpsd_text) != 0)
    {
        lv_label_set_text_fmt(ui_loginmessLabel, "Two different password");
        lv_obj_clear_flag(ui_loginmessPanel, LV_OBJ_FLAG_HIDDEN);
        lv_timer_create(hide_mess_panel, 1000, NULL);
        return;
    }

    // 追加方式打开不存在则自动创建
    FILE *fp = fopen(USER_DATA_FILE, "a");
    if (!fp)
    {
        perror("用户文件打开失败，请联系管理员检查");
        return;
    }

    // 格式化写入一行
    fprintf(fp, "\n%s,%s,%d", acc_text, psd_text, 0);
    fclose(fp);

    struct user *newuser = malloc(sizeof(struct user));
    strcpy(newuser->username, acc_text);
    strcpy(newuser->password, psd_text);
    newuser->role = 0;
    list_add_tail(&newuser->list, &gloabl_user_list);

    lv_label_set_text_fmt(ui_loginmessLabel, "Register Sucessfully.");
    lv_obj_clear_flag(ui_loginmessPanel, LV_OBJ_FLAG_HIDDEN);
    lv_timer_create(hide_mess_panel, 1000, NULL);

    // 清空注册框，防止误触
    lv_textarea_set_text(ui_Raccount, "");
    lv_textarea_set_text(ui_Rpassword, "");
    lv_textarea_set_text(ui_RpeatePassword, "");

    // 跳转登录 1:login 2:register
    lv_tabview_set_act(ui_TabView1, 1, LV_ANIM_ON);
}

// 登陆逻辑
void login_click(lv_event_t *e)
{
    // 抓取文本框数据
    const char * acc_text = lv_textarea_get_text(ui_accountText);
    const char * psd_text = lv_textarea_get_text(ui_paswdtext);

    struct user *pos;
    int login_state = 0;

    list_for_each_entry(pos, &gloabl_user_list, list)
    {
        if (strcmp(pos->username, acc_text) == 0 && strcmp(pos->password, psd_text) == 0)
        {
            login_state = 1;
            break;
        }
    }

    if (login_state == 1)
    {
        current_login_user = pos;
        lv_label_set_text_fmt(ui_loginmessLabel, "Login Sucessfully");
        lv_obj_clear_flag(ui_loginmessPanel, LV_OBJ_FLAG_HIDDEN);
        lv_timer_create(hide_mess_panel, 1000, NULL);
        _ui_screen_change(&ui_MainPage, LV_SCR_LOAD_ANIM_FADE_ON, 500, 0, ui_MainPage_screen_init);
    }
    else
    {
        lv_label_set_text_fmt(ui_loginmessLabel, "Login Failed");
        lv_obj_clear_flag(ui_loginmessPanel, LV_OBJ_FLAG_HIDDEN);
        lv_timer_create(hide_mess_panel, 1000, NULL);
    }
}

// 登录页面事件绑定
void login_page_init(void)
{
    lv_obj_add_event_cb(ui_Login, login_click, LV_EVENT_CLICKED, NULL);
    lv_obj_add_event_cb(ui_Register, register_click, LV_EVENT_CLICKED, NULL);
}
