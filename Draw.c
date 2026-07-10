#include "myhead.h"
#include "ui.h"
#include <math.h>

// 当前工具类型
typedef struct
{
    int tool_id;
    lv_color_t color;
    int thickness;
} WhiteboardTool;

// 形状全局变量
static lv_point_t start_p;
static lv_obj_t * preview_rect = NULL;
static lv_obj_t * preview_line = NULL;
static lv_point_t ghost_points[11];
static lv_obj_t * selected_tool_btn = NULL;
static lv_point_t last_p;
static bool isDrawing = false;

// 画布对象
lv_obj_t * my_canvas = NULL;
lv_color_t * canvas_buf = NULL;
int canvas_w = 1024;
int cnavas_h = 600;
lv_obj_t * canvas_bg = NULL;

// 当前工具状态
WhiteboardTool current_tool = {.tool_id = 1, .thickness = 6};

// 绘画事件
static void canvas_draw_event(lv_event_t * e)
{
    lv_event_code_t code = lv_event_get_code(e);
    lv_obj_t * canvas = lv_event_get_target(e);

    // 获取坐标并转换
    lv_indev_t * indev = lv_indev_get_act();
    lv_point_t screen_p;
    lv_indev_get_point(indev, &screen_p);
    lv_point_t current_p;
    current_p.x = screen_p.x - canvas->coords.x1;
    current_p.y = screen_p.y - canvas->coords.y1;

    // 按下
    if (code == LV_EVENT_PRESSED)
    {
        // 防漏清理
        if (preview_line) { lv_obj_del(preview_line); preview_line = NULL; }
        if (preview_rect) { lv_obj_del(preview_rect); preview_rect = NULL; }

        last_p = current_p;
        start_p = current_p;
        isDrawing = true;

        if (current_tool.tool_id >= 5)
        {
            if (current_tool.tool_id == 7 || current_tool.tool_id == 8)
            {
                // 创建一个透明obj当做预览图
                preview_rect = lv_obj_create(canvas);
                lv_obj_remove_style_all(preview_rect);
                lv_obj_set_style_bg_opa(preview_rect, LV_OPA_TRANSP, 0);
                lv_obj_set_style_border_width(preview_rect, current_tool.thickness, 0);
                lv_obj_set_style_border_color(preview_rect, current_tool.color, 0);
                if (current_tool.tool_id == 7) // 如果是圆就创建圆角
                {
                    lv_obj_set_style_radius(preview_rect, LV_RADIUS_CIRCLE, 0);
                }
            }
            else
            {
                // 创建一个线条控件作为多边形预览层
                preview_line = lv_line_create(canvas);
                lv_obj_set_style_line_width(preview_line, current_tool.thickness, 0);
                lv_obj_set_style_line_color(preview_line, current_tool.color, 0);
                lv_obj_set_style_line_rounded(preview_line, true, 0);
            }
        }
    }
    // 拖动
    else if (code == LV_EVENT_PRESSING && isDrawing)
    {
        if (current_p.x == last_p.x && current_p.y == last_p.y) return;

        // 钢笔、橡皮、刷子
        if (current_tool.tool_id >= 2 && current_tool.tool_id <= 4)
        {
            lv_draw_line_dsc_t line_dsc;
            lv_draw_line_dsc_init(&line_dsc);
            line_dsc.width = current_tool.thickness;
            line_dsc.color = current_tool.color;
            line_dsc.round_start = 1;
            line_dsc.round_end = 1;
            lv_point_t points[2] = {last_p, current_p};
            lv_canvas_draw_line(canvas, points, 2, &line_dsc);
        }
        else if (current_tool.tool_id >= 5)
        {
            int min_x = LV_MIN(start_p.x, current_p.x);
            int min_y = LV_MIN(start_p.y, current_p.y);
            int w = LV_ABS(current_p.x - start_p.x);
            int h = LV_ABS(current_p.y - start_p.y);

            if ((current_tool.tool_id == 7 || current_tool.tool_id == 8) && preview_rect)
            {
                lv_obj_set_pos(preview_rect, min_x, min_y);
                lv_obj_set_size(preview_rect, w, h);
            }
            else if (preview_line)
            {
                int pt_cnt = 0;
                if (current_tool.tool_id == 5)
                {
                    ghost_points[0] = start_p; ghost_points[1] = current_p; pt_cnt = 2;
                }
                else if (current_tool.tool_id == 6)
                {
                    ghost_points[0] = (lv_point_t){min_x + w/2, min_y};
                    ghost_points[1] = (lv_point_t){min_x + w, min_y + h};
                    ghost_points[2] = (lv_point_t){min_x, min_y + h};
                    ghost_points[3] = ghost_points[0]; pt_cnt = 4;
                }
                else if (current_tool.tool_id == 9)
                {
                    ghost_points[0] = (lv_point_t){min_x + w/2, min_y};
                    ghost_points[1] = (lv_point_t){min_x + w, min_y + h/4};
                    ghost_points[2] = (lv_point_t){min_x + w, min_y + h*3/4};
                    ghost_points[3] = (lv_point_t){min_x + w/2, min_y + h};
                    ghost_points[4] = (lv_point_t){min_x, min_y + h*3/4};
                    ghost_points[5] = (lv_point_t){min_x, min_y + h/4};
                    ghost_points[6] = ghost_points[0]; pt_cnt = 7;
                }
                else if (current_tool.tool_id == 10)
                {
                    int cx = min_x + w/2; int cy = min_y + h/2;
                    int R = LV_MIN(w, h) / 2; int r = R * 382 / 1000;
                    for (int i = 0; i < 10; i++) {
                        double angle = -3.1415926 / 2.0 + i * (3.1415926 / 5.0);
                        int rad = (i % 2 == 0) ? R : r;
                        ghost_points[i].x = cx + (int)(rad * cos(angle));
                        ghost_points[i].y = cy + (int)(rad * sin(angle));
                    }
                    ghost_points[10] = ghost_points[0]; pt_cnt = 11;
                }
                lv_line_set_points(preview_line, ghost_points, pt_cnt);
            }
        }
        last_p = current_p;
    }
    // 抬起或失去焦点
    else if (code == LV_EVENT_RELEASED || code == LV_EVENT_PRESS_LOST)
    {
        isDrawing = false;

        if (current_tool.tool_id >= 5)
        {
            int min_x = LV_MIN(start_p.x, current_p.x);
            int min_y = LV_MIN(start_p.y, current_p.y);
            int w = LV_ABS(current_p.x - start_p.x);
            int h = LV_ABS(current_p.y - start_p.y);

            if (current_tool.tool_id == 7 || current_tool.tool_id == 8)
            {
                lv_draw_rect_dsc_t rect_dsc;
                lv_draw_rect_dsc_init(&rect_dsc);
                rect_dsc.bg_opa = LV_OPA_TRANSP;
                rect_dsc.border_width = current_tool.thickness;
                rect_dsc.border_color = current_tool.color;
                if (current_tool.tool_id == 7)
                {
                    rect_dsc.radius = LV_RADIUS_CIRCLE;
                }

                if (code == LV_EVENT_RELEASED) {
                    lv_canvas_draw_rect(canvas, min_x, min_y, w, h, &rect_dsc);
                }
                if (preview_rect)
                {
                    lv_obj_del(preview_rect); preview_rect = NULL;
                }
            }
            else // 多边形和直线
            {
                int point_count = 0;
                if (current_tool.tool_id == 5)
                {
                    ghost_points[0] = start_p; ghost_points[1] = current_p; point_count = 2;
                }
                else if (current_tool.tool_id == 6) {
                    ghost_points[0] = (lv_point_t){min_x + w/2, min_y};
                    ghost_points[1] = (lv_point_t){min_x + w, min_y + h};
                    ghost_points[2] = (lv_point_t){min_x, min_y + h};
                    ghost_points[3] = ghost_points[0]; point_count = 4;
                }
                else if (current_tool.tool_id == 9) {
                    ghost_points[0] = (lv_point_t){min_x + w/2, min_y};
                    ghost_points[1] = (lv_point_t){min_x + w, min_y + h/4};
                    ghost_points[2] = (lv_point_t){min_x + w, min_y + h*3/4};
                    ghost_points[3] = (lv_point_t){min_x + w/2, min_y + h};
                    ghost_points[4] = (lv_point_t){min_x, min_y + h*3/4};
                    ghost_points[5] = (lv_point_t){min_x, min_y + h/4};
                    ghost_points[6] = ghost_points[0]; point_count = 7;
                }
                else if (current_tool.tool_id == 10) {
                    int cx = min_x + w/2; int cy = min_y + h/2;
                    int R = LV_MIN(w, h) / 2; int r = R * 382 / 1000;
                    for (int i = 0; i < 10; i++) {
                        double angle = -3.1415926 / 2.0 + i * (3.1415926 / 5.0);
                        int rad = (i % 2 == 0) ? R : r;
                        ghost_points[i].x = cx + (int)(rad * cos(angle));
                        ghost_points[i].y = cy + (int)(rad * sin(angle));
                    }
                    ghost_points[10] = ghost_points[0]; point_count = 11;
                }

                if (code == LV_EVENT_RELEASED) {
                    lv_draw_line_dsc_t line_dsc;
                    lv_draw_line_dsc_init(&line_dsc);
                    line_dsc.width = current_tool.thickness;
                    line_dsc.color = current_tool.color;
                    line_dsc.round_start = 1;
                    line_dsc.round_end = 1;

                    for (int i = 0; i < point_count - 1; i++) {
                        lv_point_t pts[2] = {ghost_points[i], ghost_points[i+1]};
                        lv_canvas_draw_line(canvas, pts, 2, &line_dsc);
                    }
                }

                if (preview_line) { lv_obj_del(preview_line); preview_line = NULL; }
            }
        }
    }
}

// 初始化画布
void init_canvas(void)
{
    current_tool.color = lv_color_hex(0x000000);

    lv_textarea_set_accepted_chars(ui_CanvasX, "0123456789");
    lv_textarea_set_accepted_chars(ui_CanvasY, "0123456789");

    canvas_bg = lv_obj_create(ui_DrawPage);
    lv_obj_remove_style_all(canvas_bg);
    lv_obj_set_size(canvas_bg, canvas_w, cnavas_h);
    lv_obj_center(canvas_bg);
    lv_obj_set_style_bg_color(canvas_bg, lv_color_hex(0xFFFFFF), LV_PART_MAIN);
    lv_obj_set_style_bg_opa(canvas_bg, LV_OPA_COVER, LV_PART_MAIN);

    my_canvas = lv_canvas_create(ui_DrawPage);

    uint32_t buf_size = canvas_w * cnavas_h * sizeof(lv_color_t);
    canvas_buf = (lv_color_t *)malloc(buf_size);

    if (canvas_buf == NULL)
    {
        printf("内存不足，画布创建失败");
        return;
    }

    lv_canvas_set_buffer(my_canvas, canvas_buf, canvas_w, cnavas_h, LV_IMG_CF_TRUE_COLOR_CHROMA_KEYED);
    lv_obj_set_size(my_canvas, canvas_w, cnavas_h);

    lv_canvas_fill_bg(my_canvas, LV_COLOR_CHROMA_KEY, LV_OPA_COVER);
    lv_obj_add_state(ui_CanvasBGbtn1, LV_STATE_CHECKED);

    lv_obj_center(my_canvas);

    // 画布边框
    lv_obj_set_style_border_width(my_canvas, 2, LV_PART_MAIN);
    lv_obj_set_style_border_color(my_canvas, lv_color_hex(0x7F7F7F), LV_PART_MAIN);

    // 触摸功能打开
    lv_obj_add_flag(my_canvas, LV_OBJ_FLAG_CLICKABLE);

    // 防止滚动
    lv_obj_clear_flag(my_canvas, LV_OBJ_FLAG_SCROLLABLE);
    lv_obj_clear_flag(canvas_bg, LV_OBJ_FLAG_SCROLLABLE);
    lv_obj_clear_flag(ui_DrawPage, LV_OBJ_FLAG_SCROLLABLE);

    // 绑定绘图函数
    lv_obj_add_event_cb(my_canvas, canvas_draw_event, LV_EVENT_ALL, NULL);
    lv_obj_move_foreground(ui_PanelToolbar);
    lv_obj_move_foreground(ui_PenSizePenal);
    lv_obj_move_foreground(ui_ColorPanel);
    lv_obj_move_foreground(ui_ShutDraw);
    lv_obj_move_foreground(ui_CanvasSettingPanel);
    lv_obj_move_foreground(ui_BgSizeKey);
    lv_obj_move_foreground(ui_ShapePanel);
}

// 形状panel切换
static void shape_switch_event(lv_event_t * e)
{
    lv_obj_t * target = lv_event_get_target(e);

    // 子菜单互斥
    lv_obj_clear_state(ui_LineBtn, LV_STATE_CHECKED);
    lv_obj_clear_state(ui_TriangelBtn, LV_STATE_CHECKED);
    lv_obj_clear_state(ui_CirculeBtn, LV_STATE_CHECKED);
    lv_obj_clear_state(ui_RectangularBtn, LV_STATE_CHECKED);
    lv_obj_clear_state(ui_HexagonBtn, LV_STATE_CHECKED);
    lv_obj_clear_state(ui_StarBtn, LV_STATE_CHECKED);

    lv_obj_add_state(target, LV_STATE_CHECKED);

    if (target == ui_LineBtn)
    {
        current_tool.tool_id = 5;
    }
    else if (target == ui_TriangelBtn)
    {
        current_tool.tool_id = 6;
    }
    else if (target == ui_CirculeBtn)
    {
        current_tool.tool_id = 7;
    }
    else if (target == ui_RectangularBtn)
    {
        current_tool.tool_id = 8;
    }
    else if (target == ui_HexagonBtn)
    {
        current_tool.tool_id = 9;
    }
    else if (target == ui_StarBtn)
    {
        current_tool.tool_id = 10;
    }
}

// 颜色修改
static void colormodify_event(lv_event_t * e)
{
    lv_obj_t *colorwheel = lv_event_get_target(e);

    lv_color_t new_color = lv_colorwheel_get_rgb(colorwheel);

    if (current_tool.tool_id != 1 || current_tool.tool_id != 3)
    {
        current_tool.color = new_color;
    }
}

// 黑色控制
static void BlackBtn_event_cb(lv_event_t * e)
{
    current_tool.color = lv_color_hex(0x000000);
}

// 工具栏切换
static void tool_switch_event(lv_event_t * e)
{
    lv_obj_t * target_btn = lv_event_get_target(e);

    // 点击已选中的按钮，取消选中
    if (target_btn == selected_tool_btn)
    {
        lv_obj_clear_state(target_btn, LV_STATE_CHECKED);
        selected_tool_btn = NULL;
        current_tool.tool_id = 0;

        lv_obj_add_flag(ui_PenSizePenal, LV_OBJ_FLAG_HIDDEN);
        lv_obj_add_flag(ui_ColorPanel, LV_OBJ_FLAG_HIDDEN);
        lv_obj_add_flag(ui_ShapePanel, LV_OBJ_FLAG_HIDDEN);
        lv_obj_add_flag(ui_ShapePanel, LV_OBJ_FLAG_HIDDEN);
        return;
    }

    // 互斥：清除其他按钮选中状态
    lv_obj_clear_state(ui_PenBtn, LV_STATE_CHECKED);
    lv_obj_clear_state(ui_earseBtn, LV_STATE_CHECKED);
    lv_obj_clear_state(ui_BushBtn, LV_STATE_CHECKED);
    lv_obj_clear_state(ui_CusrsorBtn, LV_STATE_CHECKED);
    lv_obj_clear_state(ui_ColorBtn, LV_STATE_CHECKED);
    lv_obj_clear_state(ui_CanvasSeetingsBtn, LV_STATE_CHECKED);
    lv_obj_clear_state(ui_ShapeBtn, LV_STATE_CHECKED);

    // 目标按钮选中
    lv_obj_add_state(target_btn, LV_STATE_CHECKED);
    selected_tool_btn = target_btn;

    lv_obj_add_flag(ui_PenSizePenal, LV_OBJ_FLAG_HIDDEN);
    lv_obj_add_flag(ui_ColorPanel, LV_OBJ_FLAG_HIDDEN);
    lv_obj_add_flag(ui_CanvasSettingPanel, LV_OBJ_FLAG_HIDDEN);
    lv_obj_add_flag(ui_ShapePanel, LV_OBJ_FLAG_HIDDEN);

    if (target_btn == ui_CusrsorBtn)
    {
        current_tool.tool_id = 1;
        current_tool.thickness = 0;
    }
    else if (target_btn == ui_PenBtn)
    {
        current_tool.tool_id = 2;
        lv_obj_clear_flag(ui_PenSizePenal, LV_OBJ_FLAG_HIDDEN);
    }
    else if (target_btn == ui_earseBtn)
    {
        current_tool.tool_id = 3;
        current_tool.thickness = 40;
        current_tool.color = LV_COLOR_CHROMA_KEY;
    }
    else if (target_btn == ui_BushBtn)
    {
        current_tool.tool_id = 4;
        current_tool.thickness = 40;
        current_tool.color = lv_colorwheel_get_rgb(ui_Colorwheel);
    }
    else if (target_btn == ui_ColorBtn)
    {
        lv_obj_clear_flag(ui_ColorPanel, LV_OBJ_FLAG_HIDDEN);
    }
    else if (target_btn == ui_CanvasSeetingsBtn)
    {
        lv_obj_clear_flag(ui_CanvasSettingPanel, LV_OBJ_FLAG_HIDDEN);
    }
    else if (target_btn == ui_ShapeBtn)
    {
        lv_obj_clear_flag(ui_ShapePanel, LV_OBJ_FLAG_HIDDEN);

        if (lv_obj_has_state(ui_TriangelBtn, LV_STATE_CHECKED))
        {
            current_tool.tool_id = 6;
        }
        else if (lv_obj_has_state(ui_CirculeBtn, LV_STATE_CHECKED))
        {
            current_tool.tool_id = 7;
        }
        else if (lv_obj_has_state(ui_RectangularBtn, LV_STATE_CHECKED))
        {
            current_tool.tool_id = 8;
        }
        else if (lv_obj_has_state(ui_HexagonBtn, LV_STATE_CHECKED))
        {
            current_tool.tool_id = 9;
        }
        else if (lv_obj_has_state(ui_StarBtn, LV_STATE_CHECKED))
        {
            current_tool.tool_id = 10;
        }
        else
        {
            current_tool.tool_id = 5;
            lv_obj_add_state(ui_LineBtn, LV_STATE_CHECKED);
        }
    }
}

// 画布大小设置
static void canvas_resize_confirm(lv_event_t * e)
{
    int new_W = atoi(lv_textarea_get_text(ui_CanvasX));
    int new_H = atoi(lv_textarea_get_text(ui_CanvasY));

    if (new_W < 1) new_W = 1024;
    if (new_H < 1) new_H = 600;
    if (new_W > 1024) { new_W = 1024; lv_textarea_set_text(ui_CanvasX, "1024"); }
    if (new_H > 600)  { new_H = 600;  lv_textarea_set_text(ui_CanvasY, "600"); }

    free(canvas_buf);
    canvas_buf = (lv_color_t *)malloc(new_W * new_H * sizeof(lv_color_t));
    lv_canvas_set_buffer(my_canvas, canvas_buf, new_W, new_H, LV_IMG_CF_TRUE_COLOR_CHROMA_KEYED);
    lv_obj_set_size(my_canvas, new_W, new_H);
    lv_obj_center(my_canvas);

    lv_canvas_fill_bg(my_canvas, LV_COLOR_CHROMA_KEY, LV_OPA_COVER);
    lv_obj_set_size(canvas_bg, new_W, new_H);
    lv_obj_center(canvas_bg);

    lv_canvas_fill_bg(my_canvas, lv_color_hex(0xFFFFFF), LV_OPA_COVER);

    lv_obj_move_foreground(ui_PanelToolbar);
    lv_obj_move_foreground(ui_PenSizePenal);
    lv_obj_move_foreground(ui_ColorPanel);
    lv_obj_move_foreground(ui_ShutDraw);
    lv_obj_move_foreground(ui_CanvasSettingPanel);
    lv_obj_move_foreground(ui_BgSizeKey);

    lv_obj_invalidate(my_canvas);
}

// 画布背景设置
static void CanvsBG_setting(lv_event_t * e)
{
    lv_obj_t * target_btn = lv_event_get_target(e);

    lv_obj_clear_state(ui_CanvasBGbtn1, LV_STATE_CHECKED);
    lv_obj_clear_state(ui_CanvasBGbtn2, LV_STATE_CHECKED);
    lv_obj_clear_state(ui_CanvasBGbtn3, LV_STATE_CHECKED);
    lv_obj_clear_state(ui_CanvasBGbtn4, LV_STATE_CHECKED);

    lv_obj_add_state(target_btn, LV_STATE_CHECKED);

    if (target_btn == ui_CanvasBGbtn1)
    {
        lv_obj_set_style_bg_img_src(canvas_bg, NULL, LV_PART_MAIN);
        lv_obj_set_style_bg_color(canvas_bg, lv_color_hex(0xFFFFFF), LV_PART_MAIN);
    }
    else if (target_btn == ui_CanvasBGbtn2)
    {
        lv_draw_img_dsc_t img_dsc;
        lv_draw_img_dsc_init(&img_dsc);
        lv_canvas_draw_img(my_canvas, 0, 0, &ui_img_canvasbg1_png, &img_dsc);
    }
    else if (target_btn == ui_CanvasBGbtn3)
    {
        lv_draw_img_dsc_t img_dsc;
        lv_draw_img_dsc_init(&img_dsc);
        lv_canvas_draw_img(my_canvas, 0, 0, &ui_img_canvasbg2_png, &img_dsc);
    }
    else if (target_btn == ui_CanvasBGbtn4)
    {
        lv_draw_img_dsc_t img_dsc;
        lv_draw_img_dsc_init(&img_dsc);
        lv_canvas_draw_img(my_canvas, 0, 0, &ui_img_canvasbg3_png, &img_dsc);
    }

    lv_obj_invalidate(my_canvas);
}

// 画笔大小切换
static void penSize_switch_event(lv_event_t * e)
{
    lv_obj_t * target_btn = lv_event_get_target(e);

    lv_obj_clear_state(ui_Pensize1, LV_STATE_CHECKED);
    lv_obj_clear_state(ui_Pensize2, LV_STATE_CHECKED);
    lv_obj_clear_state(ui_Pensize3, LV_STATE_CHECKED);
    lv_obj_clear_state(ui_Pensize4, LV_STATE_CHECKED);

    lv_obj_add_state(target_btn, LV_STATE_CHECKED);

    if (target_btn == ui_Pensize1)
    {
        current_tool.thickness = 2;
    }
    else if (target_btn == ui_Pensize2)
    {
        current_tool.thickness = 6;
    }
    else if (target_btn == ui_Pensize3)
    {
        current_tool.thickness = 12;
    }
    else if (target_btn == ui_Pensize4)
    {
        current_tool.thickness = 24;
    }
}

// 绘图页面事件绑定
void draw_page_init(void)
{
    lv_obj_add_event_cb(ui_PenBtn, tool_switch_event, LV_EVENT_CLICKED, NULL);
    lv_obj_add_event_cb(ui_earseBtn, tool_switch_event, LV_EVENT_CLICKED, NULL);
    lv_obj_add_event_cb(ui_BushBtn, tool_switch_event, LV_EVENT_CLICKED, NULL);
    lv_obj_add_event_cb(ui_CusrsorBtn, tool_switch_event, LV_EVENT_CLICKED, NULL);
    lv_obj_add_event_cb(ui_ColorBtn, tool_switch_event, LV_EVENT_CLICKED, NULL);
    lv_obj_add_event_cb(ui_Colorwheel, colormodify_event, LV_EVENT_VALUE_CHANGED, NULL);
    lv_obj_add_event_cb(ui_BlackBtn, BlackBtn_event_cb, LV_EVENT_CLICKED, NULL);

    lv_obj_add_event_cb(ui_Pensize1, penSize_switch_event, LV_EVENT_CLICKED, NULL);
    lv_obj_add_event_cb(ui_Pensize2, penSize_switch_event, LV_EVENT_CLICKED, NULL);
    lv_obj_add_event_cb(ui_Pensize3, penSize_switch_event, LV_EVENT_CLICKED, NULL);
    lv_obj_add_event_cb(ui_Pensize4, penSize_switch_event, LV_EVENT_CLICKED, NULL);

    lv_obj_add_event_cb(ui_CanvasSeetingsBtn, tool_switch_event, LV_EVENT_CLICKED, NULL);
    lv_obj_add_event_cb(ui_CanvasBGbtn1, CanvsBG_setting, LV_EVENT_CLICKED, NULL);
    lv_obj_add_event_cb(ui_CanvasBGbtn2, CanvsBG_setting, LV_EVENT_CLICKED, NULL);
    lv_obj_add_event_cb(ui_CanvasBGbtn3, CanvsBG_setting, LV_EVENT_CLICKED, NULL);
    lv_obj_add_event_cb(ui_CanvasBGbtn4, CanvsBG_setting, LV_EVENT_CLICKED, NULL);
    lv_obj_add_event_cb(ui_BgSizeConfirmBtn, canvas_resize_confirm, LV_EVENT_CLICKED, NULL);

    lv_obj_add_event_cb(ui_LineBtn, shape_switch_event, LV_EVENT_CLICKED, NULL);
    lv_obj_add_event_cb(ui_TriangelBtn, shape_switch_event, LV_EVENT_CLICKED, NULL);
    lv_obj_add_event_cb(ui_CirculeBtn, shape_switch_event, LV_EVENT_CLICKED, NULL);
    lv_obj_add_event_cb(ui_RectangularBtn, shape_switch_event, LV_EVENT_CLICKED, NULL);
    lv_obj_add_event_cb(ui_HexagonBtn, shape_switch_event, LV_EVENT_CLICKED, NULL);
    lv_obj_add_event_cb(ui_StarBtn, shape_switch_event, LV_EVENT_CLICKED, NULL);
    lv_obj_add_event_cb(ui_ShapeBtn, tool_switch_event, LV_EVENT_CLICKED, NULL);
}
