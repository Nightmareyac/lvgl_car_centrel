#include "lvgl/lvgl.h"
#include "lvgl/demos/lv_demos.h"
#include "lv_drivers/display/fbdev.h"
#include "lv_drivers/indev/evdev.h"
#include <unistd.h>
#include <pthread.h>
#include <time.h>
#include <sys/time.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <dirent.h>
#include "kernel_list.h"
#include <errno.h>
#include <string.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <linux/fb.h>
#include <sys/mman.h>

extern const lv_font_t chinese;

#define DISP_BUF_SIZE (1024 * 60)
#define USER_DATA_FILE "libmanage/users.txt"
#define MAX_GALLERY_IMAGES 20
#define MAX_IMG 20
#define MAX_BGIMG 10

extern struct user * current_login_user;

//全局用户头节点
extern struct list_head gloabl_user_list;
struct user
{
    char username[20];
    char password[20];
    int role;           //0:普通用户 1:管理员

    //我的截图
    struct list_head screenshots;

    //链表指针
    struct list_head list;
};



