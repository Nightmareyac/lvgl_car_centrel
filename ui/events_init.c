#include "events_init.h"
#include "map.h"   
#include "vehicle.h"    
#include "player_ui.h" 
#include "weather_ui.h"  
#include "system_ui.h"  
#include "aichat.h"
//初始化所有 UI 业务模块

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

    /* 6. 地图初始化*/
    load_ip_map_async();

    
}
