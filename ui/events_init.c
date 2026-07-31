/**
 * @file    events_init.c
 * @brief   UI 事件统一初始化入口实现
 * @details 作为所有 UI 业务模块的单一启动点，统一调用各模块的 *_init() 函数。
 *          主程序 main() 只需调用 ui_events_init()，无需关心各模块细节。
 */

#include "events_init.h"

#include "vehicle.h"     /* 车辆设备控制 + 巴法云状态同步 */
#include "player_ui.h"   /* 音乐播放器 UI */
#include "weather_ui.h"  /* 天气面板 UI */
#include "system_ui.h"   /* 系统设置（声音/亮度） */
#include "aichat.h"
/**
 * @brief 初始化所有 UI 业务模块
 * @details 各模块初始化顺序有依赖关系：
 *          1. vehicle_init 最先执行（建立 IoT 连接，为设备控制做准备）
 *          2. player_ui_init 其次（初始化音频系统）
 *          3. weather_ui_init 和 system_ui_init 无依赖，顺序任意
 */
void ui_events_init(void)
{
    /* 1. 车辆设备控制：连接巴法云 + 注册状态同步定时器 + 绑定车辆按钮事件 */
    vehicle_init();

    /* 2. 音乐播放器：初始化音频系统 + 注册进度条定时器 + 绑定播放控制事件 */
    player_ui_init();

    /* 3. 天气面板：绑定天气按钮和日历按钮事件 */
    weather_ui_init();

    /* 4. 系统设置：绑定音量滑块、亮度滑块及其按钮事件 */
    system_ui_init();

    /* 5. ai功能:ai对话*/
    ai_init();

    
}
