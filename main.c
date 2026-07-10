#include "myhead.h"
#include "ui.h"

// 内核全局用户头节点
LIST_HEAD(gloabl_user_list);

// 指向当前已登录用户
struct user * current_login_user = NULL;

// 图片库
static char img_lib[MAX_IMG][256];

// 外部函数声明
extern void User_Loading(void);
extern void login_page_init(void);
extern void init_canvas(void);
extern void draw_page_init(void);
extern void photo_page_init(void);

// LVGL 初始化
void lvgl_system_init(void)
{
    /* 1. 初始化 LVGL 核心库 */
    lv_init();

    /* 2. 初始化文件系统 */
    lv_fs_posix_init();

    /* 3. 初始化帧缓冲设备 */
    fbdev_init();

    /* 4. 配置显示缓冲区 */
    static lv_color_t buf[DISP_BUF_SIZE];
    static lv_disp_draw_buf_t disp_buf;
    lv_disp_draw_buf_init(&disp_buf, buf, NULL, DISP_BUF_SIZE);

    /* 5. 注册显示驱动 */
    static lv_disp_drv_t disp_drv;
    lv_disp_drv_init(&disp_drv);
    disp_drv.draw_buf   = &disp_buf;
    disp_drv.flush_cb   = fbdev_flush;
    disp_drv.hor_res    = 1024;
    disp_drv.ver_res    = 600;
    lv_disp_drv_register(&disp_drv);

    /* 6. 注册输入驱动 */
    evdev_init();
    static lv_indev_drv_t indev_drv_1;
    lv_indev_drv_init(&indev_drv_1);
    indev_drv_1.type = LV_INDEV_TYPE_POINTER;
    indev_drv_1.read_cb = evdev_read;
    lv_indev_t *mouse_indev = lv_indev_drv_register(&indev_drv_1);

    /* 7. 设置鼠标光标 */
    LV_IMG_DECLARE(mouse_cursor_icon);
    lv_obj_t * cursor_obj = lv_img_create(lv_scr_act());
    lv_img_set_src(cursor_obj, &mouse_cursor_icon);
    lv_indev_set_cursor(mouse_indev, cursor_obj);
}

// 时间更新
void clock_update(void)
{
    time_t now;
    struct tm tm;

    // 获取时间
    now = time(NULL);
    localtime_r(&now, &tm);

    lv_label_set_text_fmt(ui_TimeLabel, "%02d:%02d", tm.tm_hour, tm.tm_min);
    lv_label_set_text_fmt(ui_yearLabel, "%04d/%02d/%02d", tm.tm_year + 1900, tm.tm_mon + 1, tm.tm_mday);
    char week_buf[8];
    strftime(week_buf, sizeof(week_buf), "%a", &tm);
    lv_label_set_text(ui_weekLabel, week_buf);
}

int main(void)
{
    User_Loading();

    lvgl_system_init();

    ui_init();

    // 初始化画布
    init_canvas();

    // 绑定各页面事件
    login_page_init();
    draw_page_init();
    photo_page_init();
    Seeting_page_init();

    // 启动时钟更新定时器
    lv_timer_create(clock_update, 1000, NULL);

    while (1) {
        lv_timer_handler();
        usleep(5000);
    }

    return 0;
}

uint32_t custom_tick_get(void)
{
    static uint64_t start_ms = 0;
    if (start_ms == 0) {
        struct timeval tv_start;
        gettimeofday(&tv_start, NULL);
        start_ms = (tv_start.tv_sec * 1000000 + tv_start.tv_usec) / 1000;
    }
    struct timeval tv_now;
    gettimeofday(&tv_now, NULL);
    uint64_t now_ms;
    now_ms = (tv_now.tv_sec * 1000000 + tv_now.tv_usec) / 1000;

    uint32_t time_ms = now_ms - start_ms;

    return time_ms;
}
