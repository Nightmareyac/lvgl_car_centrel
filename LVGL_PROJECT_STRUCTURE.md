# sq_lv2 UI/LVGL 项目结构说明

## 1. 项目概况

本文档整理 RK1808 开发板上的 sq_lv2 UI 项目，源码目录为：

D:\myrepo1\sq_lv2\ui

运行环境是 RK1808 Linux/AArch64 开发板，使用 framebuffer 显示、evdev 输入、ALSA 音频和 LVGL 8.x 风格 API。界面分辨率为 1024 x 600。

功能包括车辆控制、巴法云状态同步、mplayer 音乐播放、天气和日历、AI 对话、音量控制、RK1808 背光控制以及面板动画。

## 2. 目录结构

~~~text
sq_lv2/
├─ main.c                         # LVGL、framebuffer、evdev 和主循环
├─ CMakeLists.txt                 # 构建配置
├─ lv_conf.h / lv_drv_conf.h      # LVGL 和驱动配置
├─ lvgl/                          # LVGL 源码
├─ lv_drivers/                    # framebuffer 和 evdev 驱动
└─ ui/
   ├─ ui.c / ui.h                 # UI 总入口和屏幕切换
   ├─ screens.c / screens.h       # EEZ Studio 生成的控件创建代码
   ├─ styles.c / styles.h         # EEZ Studio 生成的样式
   ├─ images.c / images.h         # 图片描述符索引
   ├─ ui_image_*.c                # 图片资源
   ├─ ui_font_mi_regular.c        # 字体资源
   ├─ fonts.h / vars.h / structs.h # 资源和扩展声明
   ├─ anim.c / anim.h              # 面板动画
   ├─ events_init.c / .h           # 业务模块统一初始化
   ├─ vehicle.c / vehicle.h        # 车辆控制、云端同步、时间显示
   ├─ bemfa_client.c / .h          # 巴法云 TCP 客户端和接收线程
   ├─ music.c / music.h            # mplayer、FIFO 和播放状态
   ├─ player_ui.c / player_ui.h    # 播放器 UI
   ├─ timePanel.c / timePanel.h    # 天气请求、解析和数据结构
   ├─ weather_ui.c / .h            # 天气和日历 UI
   ├─ system_ui.c / .h             # 音量和亮度 UI
   ├─ aichat.c / aichat.h          # AI 请求和后台线程
   └─ cJSON.c / cJSON.h             # JSON 解析库
~~~

分层关系：

| 层次 | 文件 | 责任 |
|---|---|---|
| 平台启动层 | main.c | 初始化 LVGL、显示、输入和主循环 |
| UI 框架层 | ui.c、screens.c、styles.c | 创建对象、屏幕加载、样式和 objects |
| 资源层 | images.c、ui_image_*.c、ui_font_mi_regular.c | 图片和字体 |
| 初始化层 | events_init.c | 统一调用各业务模块初始化 |
| UI 业务层 | vehicle.c、player_ui.c、weather_ui.c、system_ui.c、aichat.c | 事件和界面状态 |
| 服务层 | bemfa_client.c、music.c、timePanel.c | 网络、音频进程、HTTP 和 JSON |
| 公共辅助层 | anim.c | LVGL 动画封装 |

## 3. RK1808 启动链路

~~~text
main()
  ├─ lvgl_system_init()
  │   ├─ lv_init()
  │   ├─ fbdev_init()
  │   ├─ lv_disp_draw_buf_init()
  │   ├─ 注册 fbdev_flush 显示驱动
  │   ├─ evdev_set_file("/dev/input/event2")
  │   ├─ evdev_init()
  │   └─ 注册 evdev_read 输入驱动
  ├─ ui_init()
  │   ├─ create_screens()
  │   │   └─ create_screen_main()
  │   └─ loadScreen(SCREEN_ID_MAIN)
  ├─ ui_events_init()
  │   ├─ vehicle_init()
  │   ├─ player_ui_init()
  │   ├─ weather_ui_init()
  │   ├─ system_ui_init()
  │   └─ ai_init()
  └─ while (1)
      ├─ lv_timer_handler()
      └─ usleep(5000)
~~~

RK1808 相关运行时依赖：

| 依赖 | 当前配置 |
|---|---|
| 显示 | Linux framebuffer，fbdev |
| 输入 | evdev，当前为 /dev/input/event2 |
| 音频 | mplayer，ALSA 参数 hw=1.0 |
| 背光 | /sys/devices/platform/backlight/backlight/backlight/brightness |
| 编译 | AArch64 交叉编译器 |
| 输出 | build/bin/demo |

## 4. 主线程逻辑

主线程是 LVGL 的正常执行线程：

~~~text
lv_timer_handler()
  -> evdev 输入处理
  -> 控件事件回调
  -> LVGL 定时器回调
  -> LVGL 动画推进
  -> lv_async_call() 回调
  -> framebuffer 刷新
  -> usleep(5 ms)
  -> 重复
~~~

主线程定时任务：

| 回调 | 周期 | 作用 |
|---|---:|---|
| bemfa_ui_update_cb | 100 ms | 云端设备状态同步 |
| time_update_timer_cb | 1000 ms | 更新时间和日期 |
| progress_timer_cb | 1000 ms | 更新音乐进度 |
| LVGL 动画回调 | 动画期间 | 更新面板坐标 |
| ai_update_answer_cb | 异步 | 更新 AI 回复 |

LVGL 定时器不是独立线程，只有 main.c 周期调用 lv_timer_handler() 时才会执行。

## 5. 所有线程和进程

### 5.1 AI 网络线程

文件：aichat.c  
入口：ai_thread_func()  
创建：ai_init()

使用 g_pending_mutex 保护待发送文本，使用 g_pending_cond 让线程在无任务时休眠。

~~~text
ai_init()
  -> 注册输入框、键盘、发送和关闭事件
  -> curl_global_init()
  -> pthread_create(ai_thread_func)

用户点击发送
  -> ai_send_btn_cb()
  -> 获取文本并显示 Thinking...
  -> mutex 加锁，复制到 g_pending_input
  -> 设置 g_pending_ready
  -> pthread_cond_signal()
  -> 解锁并清空输入框

ai_thread_func()
  -> cond_wait() 等待任务
  -> 复制任务到局部缓冲区
  -> ai_send_request()
       -> cJSON 构造请求
       -> curl_easy_perform() 发送 HTTPS
       -> ai_curl_write_cb() 收集响应
       -> ai_parse_response() 解析 choices[0].message.content
  -> ai_display_response()
  -> lv_async_call(ai_update_answer_cb, copy)

主线程下一轮 lv_timer_handler()
  -> ai_update_answer_cb()
  -> lv_label_set_text(objects.ai_answer, text)
  -> free(text)
~~~

AI 线程不直接操作 LVGL，通过 lv_async_call() 安全地把结果交回主线程。当前每次请求独立，不保存对话历史；快速连续发送可能覆盖单个待发送缓冲区。

### 5.2 巴法云接收线程

文件：bemfa_client.c  
入口：bemfa_recv_thread_func()  
创建：bemfa_connect()

~~~text
vehicle_init()
  -> bemfa_connect()
       -> DNS 解析 tcp.bemfa.com
       -> socket() / connect()
       -> bemfa_subscribe_all()
       -> g_running = true
       -> pthread_create(bemfa_recv_thread_func)

bemfa_recv_thread_func()
  -> 检查 30 秒心跳
  -> 设置 2 秒 recv 超时
  -> recv()
  -> pong 直接忽略
  -> 设备消息交给 parse_bemfa_msg()
       -> 提取 topic 和 msg
       -> 修改 g_bemfa_state
       -> 设置 is_updated = true
  -> 断开时等待并尝试重连

主线程每 100 ms
  -> bemfa_ui_update_cb()
  -> 检查 is_updated
  -> 更新控件 LV_STATE_CHECKED
  -> 清除 is_updated
~~~

状态映射：

| 主题 | 状态字段 | 控件 |
|---|---|---|
| mTJT3Afrk006 | trunk_open | objects.trunk_btn |
| vc8lsGpsp006 | frunk_open | objects.funk_btn |
| DyQL1Jcsc004 | lock_locked | objects.lock_btn |
| 61SOh2Izr005 | ac_on | objects.airconditionr_call_btn |
| 4lx3WVhUA002 | flash1_turn | objects.flash1 |
| 6MHL9hbRf002 | flash2_turn | objects.flash2 |
| EIHBf8Q8A002 | flash3_turn | objects.flash3 |

接收线程不直接访问 LVGL，但 g_bemfa_state 在接收线程写、主线程读写，当前没有 mutex 或原子保护，存在数据竞争。

### 5.3 天气请求线程

文件：weather_ui.c、timePanel.c  
入口：weather_thread_func()  
创建：wealth_btn_cb()

~~~text
用户点击天气
  -> wealth_btn_cb()
  -> 滑入 wealth_panel
  -> pthread_create(weather_thread_func)
  -> pthread_detach()

weather_thread_func()
  -> fetch_weather_date("武汉", &info)
       -> 构造 POST body
       -> curl_easy_perform()
       -> write_cb() 收集响应
       -> cJSON_Parse()
       -> 解析 city 和前三天数据
  -> update_weather_ui(&info)
       -> 转换天气文字
       -> 选择天气图标
       -> 更新三个标签和三个图片

关闭天气
  -> slide_out_panel()
  -> 动画结束后隐藏 wealth_panel
~~~

重要问题：weather_thread_func() 在后台线程直接调用 update_weather_ui()，而该函数调用 lv_label_set_text_fmt() 和 lv_img_set_src()。LVGL 对象操作应在主线程执行。建议天气线程只填充数据，再使用 lv_async_call() 或 LVGL 定时器更新界面。

天气按钮每次打开都会新建线程，没有去重、取消和结果过期判断。

### 5.4 mplayer 音频子进程

music.c 不创建 POSIX 线程，而是使用 fork() 和 execlp()：

~~~text
播放或切歌
  -> start_player(path)
  -> stop_player() 结束旧进程
  -> mkfifo("/tmp/mplayer_fifo")
  -> fork()
  -> 子进程执行 mplayer slave 模式
  -> 父进程保存 mplayer_pid 和播放时间
~~~

- 音量：向 FIFO 写入 volume N 1。
- 暂停：发送 SIGSTOP。
- 继续：发送 SIGCONT。
- 切歌：停止旧进程后启动新歌曲。
- 进度：根据 play_start_time、pause_elapsed 和歌曲时长计算。
- music_init() 设置 SIGCHLD 为 SIG_IGN，避免僵尸进程。

## 6. 业务模块执行逻辑

### 6.1 车辆 vehicle.c

~~~text
vehicle_init()
  -> bemfa_connect()
  -> 创建 100 ms 状态同步定时器
  -> 创建 1000 ms 时间定时器
  -> 注册车辆按钮 LV_EVENT_CLICKED
  -> 空调按钮增加 LV_OBJ_FLAG_CHECKABLE
~~~

关键回调：

- action_trunk_btn_click：翻转后备箱并发送 on/off。
- action_frunk_btn_click：翻转前备箱并发送 on/off。
- action_lock_btn_click：发送 lock/unlock。
- action_flash1_btn_click、action_flash2_btn_click、action_flash3_btn_click：发送车灯 on/off。
- action_airconditionr_call_btn_click：显示温度滚轮，或读取温度并发送。

本地回调只下发命令，最终状态以巴法云回执为准。

### 6.2 播放器 player_ui.c 和 music.c

~~~text
player_ui_init()
  -> music_init()
  -> 创建 1 秒进度定时器
  -> 隐藏 player_main_panel
  -> build_songlist() 设置 roller
  -> upadate_musicUI() 更新当前歌曲
  -> 注册小组件和全屏控制事件
  -> 注册歌曲列表按钮
~~~

交互链路：

- play_pause_cb -> music_play_toggle -> upadate_musicUI。
- next_song_cb -> music_next -> 更新歌曲 UI。
- prev_song_cb -> music_prev -> 更新歌曲 UI。
- songlist_item_cb -> lv_roller_get_selected -> music_jump_to。
- expand_main_panel -> slide_in_panel。
- collapse_main_panel -> slide_out_panel。
- progress_timer_cb -> music_get_progress -> lv_bar_set_value。

### 6.3 天气和日历

weather_ui_init() 默认隐藏 wealth_panel 和 calander，并注册天气、日历按钮。

数据结构：

~~~c
typedef struct {
    char date[16];
    char wea[32];
    char tem1[8];
    char tem2[8];
} DayWeather;

typedef struct {
    char city[32];
    DayWeather days[3];
} WealthInfo;
~~~

fetch_weather_date() 使用 libcurl POST，请求成功后用 cJSON 读取 data.data.city 和 data 数组，最多保留三天。get_weather_icon() 将天气文字映射为 sun.bmp、rain.bmp、snow.bmp 或 cloud.bmp。

### 6.4 系统设置 system_ui.c

- sound_bar 默认隐藏，范围 0..100。
- brightness_bar 默认隐藏，范围 10..255。
- sound_bar_cb 读取滑块值并调用 music_set_volume。
- brightness_slider_cb 读取滑块值并写入 RK1808 背光 sysfs。
- sound_btn_cb 和 brightness_btn_cb 切换滑块。

### 6.5 AI 对话 aichat.c

- ai_userrequest_cb：输入框点击或聚焦时显示键盘。
- ai_keybord_cb：READY 或 CANCEL 时隐藏键盘。
- ai_send_btn_cb：提交输入并清空文本框。
- ai_chatbox_shut_cb：隐藏聊天框。
- ai_parse_response：解析 choices[0].message.content 或 error.message。

## 7. 生成代码、样式和资源

screens.c 中的 create_screens() 初始化默认主题并调用 create_screen_main()。create_screen_main() 创建主屏幕和控件，把指针保存到全局 objects 结构体，业务代码通过 objects.xxx 访问控件。

当前只有 SCREEN_ID_MAIN。tick_screen_main() 为空，ui_tick() 没有在 main() 中显式调用；动态行为主要由 LVGL 定时器和事件回调驱动。

styles.c 当前包含：

- click_style：按钮默认、按下、聚焦和选中状态。
- checkstyle：车锁锁定/解锁图标。
- darkslider：音量和亮度滑块。
- FRUNK：车辆背景图。

images.c 注册 23 个图片描述符，ui_image_*.c 定义图片数据，ui_font_mi_regular.c 定义字体资源。车辆大图等资源会增加 RK1808 上的可执行文件和内存占用。

## 8. 关键函数和 LVGL API

| 函数/API | 用途 |
|---|---|
| lv_init | 初始化 LVGL |
| lv_disp_draw_buf_init | 初始化显示缓冲 |
| lv_disp_drv_register | 注册 framebuffer 显示驱动 |
| lv_indev_drv_register | 注册 evdev 输入驱动 |
| create_screens / create_screen_main | 创建主题、屏幕和控件 |
| loadScreen | 使用 lv_scr_load_anim 切屏 |
| lv_timer_handler | 驱动事件、定时器、动画和刷新 |
| lv_obj_add_event_cb | 注册控件事件 |
| lv_event_get_target / lv_event_get_code | 获取事件控件和类型 |
| lv_obj_add_flag / lv_obj_clear_flag | 显示或隐藏对象 |
| lv_obj_add_state / lv_obj_clear_state | 设置控件选中状态 |
| lv_obj_has_state | 读取控件状态 |
| lv_label_set_text / lv_label_set_text_fmt | 更新文字 |
| lv_img_set_src | 更新图片 |
| lv_bar_set_value | 更新进度 |
| lv_slider_get_value | 读取滑块 |
| lv_roller_get_selected_str | 读取温度或歌曲选项 |
| lv_timer_create | 创建主线程定时器 |
| lv_async_call | 将后台结果交给主线程 |
| lv_anim_init / lv_anim_start | 创建并启动动画 |
| pthread_create | 创建业务线程 |
| pthread_cond_wait / pthread_cond_signal | AI 任务等待和唤醒 |
| curl_easy_perform | 执行网络请求 |
| cJSON_Parse | 解析 JSON |
| bemfa_send_msg | 下发车辆命令 |
| music_play_toggle / music_next / music_prev | 播放控制 |
| music_set_volume | 设置 mplayer 音量 |
| music_get_progress | 计算播放进度 |

## 9. 构建关系

根目录 CMakeLists.txt 递归收集 ui 下所有 C 文件，然后和 main.c 一起编译为 demo。主要链接：

- lvgl、lvgl::drivers：LVGL 核心和驱动。
- pthread：后台线程。
- curl、ssl、crypto、z：HTTPS 和压缩依赖。
- m、dl：数学和动态加载依赖。

RK1808 使用 AArch64 交叉编译器，输出目标为 build/bin/demo。

## 10. 当前风险和维护建议

1. 天气线程直接调用 LVGL，应改为后台线程写数据、主线程更新控件。
2. g_bemfa_state 存在跨线程读写，建议增加 mutex、原子变量或消息队列。
3. AI、巴法云和天气访问凭据硬编码在源码中，建议改为部署配置并轮换已暴露凭据。
4. AI 和天气请求关闭 HTTPS 证书校验，生产环境应开启 CA 校验。
5. AI 和巴法云线程没有完整退出和 join 流程。
6. 每次打开天气面板都会创建新线程，没有请求去重或取消。
7. build_songlist() 使用 strcat() 写固定 512 字节缓冲区，歌曲增加后可能溢出。
8. main() 未调用 ui_tick()；当前 tick_screen_main() 为空，后续生成代码增加动态逻辑时需补上。
9. EEZ 生成文件和业务代码混放，重新生成 screens.c、images.c 或头文件时可能覆盖手工修改。

维护原则：

- 所有 LVGL 对象、样式、标签、图片和动画操作统一在主线程执行。
- 后台线程只执行阻塞 I/O、协议解析和纯数据处理。
- 后台结果通过 mutex 保护的数据副本、消息队列或 lv_async_call() 交给主线程。
- 周期性 UI 刷新优先使用 lv_timer_create()。
- 事件回调只提交任务或修改轻量状态，不执行长时间网络请求。
- EEZ 生成代码和手工业务代码分开维护。

