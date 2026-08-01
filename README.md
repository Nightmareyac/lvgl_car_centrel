# RK1808 LVGL 车载控制界面

本项目基于 LVGL，运行在 RK1808 Linux framebuffer 环境，主要实现 AI 对话、
音乐播放和车辆远程控制等功能。

## 获取源码

```bash
git clone https://github.com/Nightmareyac/lvgl_car_centrel.git
cd lvgl_car_centrel
git submodule update --init --recursive
```

## 1. AI 对话逻

核心文件：`ui/aichat.c`

AI 模块采用“LVGL 事件回调 + 后台线程”的方式：

1. 用户在输入框输入问题并点击发送按钮。
2. `ai_send_btn_cb()` 获取输入内容，并通过互斥锁保护待发送数据。
3. 条件变量唤醒 `ai_thread_func()` 后台线程。
4. 后台线程使用 curl 向 AI 接口发送请求。
5. 使用 cJSON 解析 `choices[0].message.content` 中的回复内容。
6. 通过 `lv_async_call()` 回到 LVGL 线程，更新答案标签。

网络请求不直接操作 LVGL 控件，避免后台线程引发界面线程安全问题。

## 2. 音乐播放器逻辑

核心文件：`ui/player_ui.c`、`ui/music.c`

`player_ui.c` 负责界面和按钮事件，`music.c` 负责底层音频播放。

播放器按钮的调用流程是：

```text
LVGL 按钮事件
    -> player_ui.c 回调
    -> music.c 播放控制函数
    -> mplayer 进程或 FIFO
    -> 刷新播放器界面
```

`music.c` 通过 `fork()` 和 `execlp()` 启动 mplayer 的 slave 模式，并通过
`/tmp/mplayer_fifo` 发送音量等控制命令。播放暂停使用 `SIGSTOP` 和 `SIGCONT`，
切歌时重新启动 mplayer。

`music_all_panel` 会被移动到 `lv_layer_top()`，因此播放器可以跨主页面和地图
页面显示。`flash1` 到 `flash4` 放在该公共面板中，也可以随播放器一起复用。

## 3. 巴法云车辆控制逻辑

核心文件：`ui/bemfa_client.c`、`ui/vehicle.c`

`bemfa_client.c` 负责 TCP 网络通信，`vehicle.c` 负责 LVGL 按钮事件和界面状态。

车辆控制流程是：

```text
点击车辆按钮
    -> vehicle.c 事件回调
    -> bemfa_send_msg() 发送主题消息
    -> 巴法云返回设备状态
    -> 接收线程解析 topic 和 msg
    -> 更新 g_bemfa_state
    -> LVGL 定时器刷新按钮状态
```

Bemfa 接收线程负责：

- 接收云端推送消息
- 过滤 `pong` 心跳回复
- 解析车辆主题和状态
- 检测连接断开并尝试重连

LVGL 控件不会直接在 Socket 接收线程中修改，而是通过共享状态和
`bemfa_ui_update_cb()` 定时同步。

## 4. 地图功能逻辑

核心文件：`ui/map.c`

地图接口返回 JSON，图片数据保存在 `msg` 字段中。程序依次完成：

```text
下载 JSON
    -> 解析 code 和 msg
    -> Base64 解码 PNG
    -> stb_image 解析 PNG
    -> 转换为 BMP
    -> 通过 lv_async_call() 刷新 LVGL 地图
```

地图下载在后台线程执行，UI 更新回到 LVGL 线程执行。地图图片放在背景层，
退出按钮提升到前景，避免地图覆盖控件。

## 5. 模块化设计

项目将界面和业务逻辑分开：

- EEZ Studio 负责生成页面布局
- `aichat.c` 负责 AI 网络和对话流程
- `player_ui.c` 负责播放器 UI
- `music.c` 负责音频播放
- `bemfa_client.c` 负责云端通信
- `vehicle.c` 负责车辆界面控制
- `map.c` 负责地图下载和转换

这种结构可以减少模块之间的耦合，便于后续增加设备控制、播放器功能和网络服务。
