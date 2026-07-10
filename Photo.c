#include "myhead.h"
#include "ui.h"

extern struct user * current_login_user;

static int gallery_count = 0;
static int current_img_idx = -1;
static char gallery_files[MAX_GALLERY_IMAGES][256];
static lv_img_dsc_t custom_img_dsc;
static uint8_t * custom_img_data = NULL;

// 存储用户截图
static void load_gallery_file(void)
{
    gallery_count = 0;
    current_img_idx = -1;
    char search_path[256];

    if (current_login_user->role == 1)
    {
        strcpy(search_path, "/userdata/screenshots/");
    }
    else
    {
        sprintf(search_path, "/userdata/screenshots/%s/", current_login_user->username);
    }

    DIR *dir = opendir(search_path);
    if (dir != NULL)
    {
        struct dirent *ent;
        while ((ent = readdir(dir)) != NULL && gallery_count < MAX_GALLERY_IMAGES)
        {
            if (strstr(ent->d_name, ".bmp"))
            {
                sprintf(gallery_files[gallery_count], "%s%s", search_path, ent->d_name);
                gallery_count++;
            }
        }
        closedir(dir);
    }
}

//预览更新
static void update_preview_image(void)
{
    if (gallery_count == 0 || current_img_idx < 0) {
        lv_img_set_src(ui_photoShow, NULL);
        return;
    }
    
    // 让滚轮同步
    if (lv_roller_get_selected(ui_photoLib) != current_img_idx)
    {
        lv_roller_set_selected(ui_photoLib,current_img_idx,LV_ANIM_OFF);
    }
    
    lv_roller_set_selected(ui_photoLib, current_img_idx, LV_ANIM_OFF);

    // 拿到文件路径
    const char * filepath = gallery_files[current_img_idx];

    int bmp_fd = open(filepath, O_RDONLY);
    if (bmp_fd < 0) {
        printf("打开 bmp 图片失败: %s\n", filepath);
        return;
    }

    int bmp_w = 0, bmp_h = 0; 
    unsigned int data_offset = 0; 

    lseek(bmp_fd, 10, SEEK_SET); read(bmp_fd, &data_offset, 4);
    lseek(bmp_fd, 18, SEEK_SET); read(bmp_fd, &bmp_w, 4);
    lseek(bmp_fd, 22, SEEK_SET); read(bmp_fd, &bmp_h, 4);

    // 处理 BMP 负数高度（避免 malloc 崩溃）
    int is_top_down = 0;
    if (bmp_h < 0) {
        bmp_h = -bmp_h; 
        is_top_down = 1; 
    }

    int bmp_line_length = (bmp_w * 3 + 3) & ~3; 
    int bmp_data_size = bmp_line_length * bmp_h;

    // 清理上一张图的内存
    if (custom_img_data != NULL) {
        free(custom_img_data);
        custom_img_data = NULL;
    }
    // 强制分配 4字节/像素 内存池
    int lvgl_data_size = bmp_w * bmp_h * 4; 
    custom_img_data = (uint8_t *)malloc(lvgl_data_size);
    if (!custom_img_data) {
        printf("内存不足无法分配");
        close(bmp_fd);
        return;
    }

    unsigned char *bmp_buf = (unsigned char *)malloc(bmp_data_size);
    lseek(bmp_fd, data_offset, SEEK_SET);
    read(bmp_fd, bmp_buf, bmp_data_size);
    close(bmp_fd);

    // 2. 根据高度正负，智能决定是否颠倒画面
    for (int y = 0; y < bmp_h; y++)
    {
        for (int x = 0; x < bmp_w; x++)
        {
            int src_y = is_top_down ? y : (bmp_h - 1 - y);
            int bmp_index = (src_y * bmp_line_length) + (x * 3);
            int lv_index = (y * bmp_w + x) * 4;
            
            custom_img_data[lv_index + 0] = bmp_buf[bmp_index + 0]; // 蓝色
            custom_img_data[lv_index + 1] = bmp_buf[bmp_index + 1]; // 绿色
            custom_img_data[lv_index + 2] = bmp_buf[bmp_index + 2]; // 红色
            custom_img_data[lv_index + 3] = 0xFF;                   // Alpha 占位，强行变不透明
        }
    }
    free(bmp_buf);

    custom_img_dsc.header.always_zero = 0;
    custom_img_dsc.header.w = bmp_w;
    custom_img_dsc.header.h = bmp_h;
    // 改回 TRUE_COLOR
    custom_img_dsc.header.cf = LV_IMG_CF_TRUE_COLOR; 
    custom_img_dsc.data_size = lvgl_data_size;
    custom_img_dsc.data = custom_img_data;

    // 清空LVGL图片缓存并且重新读取数据
    lv_img_cache_invalidate_src(NULL);
    // 推送给相框
    lv_img_set_src(ui_photoShow, &custom_img_dsc);
}

/*static void update_preview_image(void)
{
    if (gallery_count == 0 || current_img_idx < 0) {
        lv_img_set_src(ui_photoShow, NULL);
        return;
    }
    
    static char lvgl_path[300];
    sprintf(lvgl_path, "S:%s", gallery_files[current_img_idx]);

    lv_img_cache_invalidate_src(NULL);

    lv_img_set_src(ui_photoShow, lvgl_path);
}*/

// 相册更新
static void updata_gallery_ui(void)
{
    if (gallery_count == 0)
    {
        lv_roller_set_options(ui_photoLib,"Empty",LV_ROLLER_MODE_NORMAL);
        lv_img_set_src(ui_photoShow,NULL);
        return;
    }

    char option_str[4096] = {0};
    for (int i = 0; i < gallery_count; i++)
    {
        char * filename = strrchr(gallery_files[i],'/');
        if (filename != NULL)
        {
            filename = filename + 1;//无效就跳过
        }
        else
        {
        filename = gallery_files[i];
        }
        
        strcat(option_str , filename);

        if (i < gallery_count -1 ) strcat(option_str, "\n");
    }
    //字符串赋值给滚轮
    lv_roller_set_options(ui_photoLib,option_str,LV_ROLLER_MODE_NORMAL);
    
   if (current_img_idx >= 0 && current_img_idx <gallery_count)
    {
        //调用硬解码
        update_preview_image();
    }
}

// 截图保存
static void save_bmp(const char * filepath)
{
    int fb_fd = open("/dev/fb0", O_RDWR);
    if (fb_fd < 0) 
    return;

    struct fb_var_screeninfo vinfo;
    struct fb_fix_screeninfo finfo;
    ioctl(fb_fd, FBIOGET_VSCREENINFO, &vinfo);
    ioctl(fb_fd, FBIOGET_FSCREENINFO, &finfo);

    long screensize = vinfo.yres_virtual * finfo.line_length;
    uint8_t *fbp = (uint8_t *)mmap(0, screensize, PROT_READ | PROT_WRITE, MAP_SHARED, fb_fd, 0);
    if (fbp == (uint8_t *)-1) {
        close(fb_fd);
        return;
    }

    int scale = 2; 
    int new_w = vinfo.xres / scale;
    int new_h = vinfo.yres / scale;

    FILE *f = fopen(filepath, "wb");
    if (!f) {
        munmap(fbp, screensize);
        close(fb_fd);
        return;
    }

    // 计算对齐和文件大小
    int row_padding = (4 - ((new_w * 3) % 4)) % 4;
    int file_size = 54 + (new_w * 3 + row_padding) * new_h;

    uint8_t bmp_header[54] = {0};
    bmp_header[0] = 'B'; bmp_header[1] = 'M';
    bmp_header[2] = file_size & 0xFF; bmp_header[3] = (file_size >> 8) & 0xFF;
    bmp_header[4] = (file_size >> 16) & 0xFF; bmp_header[5] = (file_size >> 24) & 0xFF;
    bmp_header[10] = 54;
    bmp_header[14] = 40;
    bmp_header[18] = new_w & 0xFF; bmp_header[19] = (new_w >> 8) & 0xFF;
    bmp_header[20] = (new_w >> 16) & 0xFF; bmp_header[21] = (new_w >> 24) & 0xFF;

    int32_t neg_h = -new_h;
    bmp_header[22] = neg_h & 0xFF; bmp_header[23] = (neg_h >> 8) & 0xFF;
    bmp_header[24] = (neg_h >> 16) & 0xFF; bmp_header[25] = (neg_h >> 24) & 0xFF;
    bmp_header[26] = 1;
    bmp_header[28] = 24;

    fwrite(bmp_header, 1, 54, f);

    // 抓取像素，实现物理压缩
    for (int y = 0; y < new_h; y++) {
        for (int x = 0; x < new_w; x++) {
            
            // 根据缩放因子，映射回原图的真实物理坐标
            int orig_x = x * scale;
            int orig_y = y * scale;

            long location = (orig_x + vinfo.xoffset) * (vinfo.bits_per_pixel / 8) +
                            (orig_y + vinfo.yoffset) * finfo.line_length;

            uint8_t bgr[3] = {0};
            if (vinfo.bits_per_pixel == 32) {
                bgr[0] = fbp[location + 0];
                bgr[1] = fbp[location + 1];
                bgr[2] = fbp[location + 2];
            } else if (vinfo.bits_per_pixel == 16) {
                uint16_t pixel = *(uint16_t*)(fbp + location);
                bgr[0] = (pixel & 0x001F) << 3;
                bgr[1] = ((pixel >> 5) & 0x003F) << 2;
                bgr[2] = ((pixel >> 11) & 0x001F) << 3;
            }
            fwrite(bgr, 1, 3, f);
        }
        // 补齐 4 字节倍数的行尾填充
        uint8_t pad[3] = {0};
        fwrite(pad, 1, row_padding, f);
    }

    fclose(f);
    munmap(fbp, screensize);
    close(fb_fd);
}


// 照片手动滑动选中
static void PhotoLib_change_event(lv_event_t * e)
{
    current_img_idx = lv_roller_get_selected(ui_photoLib);
    update_preview_image();
}

// 上一张
static void prev_btn_event(lv_event_t * e)
{
    if (gallery_count <= 0)
    {
        return;
    }
    current_img_idx--;
    if (current_img_idx < 0)
    {
        current_img_idx = gallery_count - 1;
    }
    update_preview_image();
}

// 下一张
static void next_btn_event(lv_event_t * e)
{
    if (gallery_count <= 0)
    {
        return;
    }
    current_img_idx++;
    if (current_img_idx >= gallery_count)
    {
        current_img_idx = 0;
    }
    update_preview_image();
}

// 删除照片
static void delete_btn_event(lv_event_t * e)
{
    if (gallery_count <= 0 || current_img_idx < 0)
    {
        return;
    }

    remove(gallery_files[current_img_idx]);
    printf("成功删除%s\n", gallery_files[current_img_idx]);

    load_gallery_file();
    if (current_img_idx >= gallery_count)
    {
        current_img_idx = gallery_count - 1;
    }
    updata_gallery_ui();
}

// 截图按钮回调
static void screenshot_btn_event(lv_event_t * e)
{
    char user_dir[256];
    sprintf(user_dir, "/userdata/screenshots/%s", current_login_user->username);
    mkdir("/userdata/screenshots", 0755);
    mkdir(user_dir, 0755);

    char file_path[256];
    time_t now = time(NULL);
    sprintf(file_path, "/userdata/screenshots/%s/img_%ld.bmp", current_login_user->username, (long)now);

    save_bmp(file_path);
}

// 文件按钮事件
static void filefolder_btn_event(lv_event_t * e)
{
    if (current_login_user == NULL) return;

    load_gallery_file();

    if (gallery_count > 0)
    {
        current_img_idx = 0;
    }

    updata_gallery_ui();

}

// 相册页面事件绑定
void photo_page_init(void)
{
    lv_obj_add_event_cb(ui_ScreenShotBtn, screenshot_btn_event, LV_EVENT_CLICKED, NULL);
    lv_obj_add_event_cb(ui_FileFolderBtn, filefolder_btn_event, LV_EVENT_CLICKED, NULL);
    lv_obj_add_event_cb(ui_photoLib, PhotoLib_change_event, LV_EVENT_VALUE_CHANGED, NULL);
    lv_obj_add_event_cb(ui_prevBtn, prev_btn_event, LV_EVENT_CLICKED, NULL);
    lv_obj_add_event_cb(ui_NextBtn, next_btn_event, LV_EVENT_CLICKED, NULL);
    lv_obj_add_event_cb(ui_DeleteBtn, delete_btn_event, LV_EVENT_CLICKED, NULL);
}