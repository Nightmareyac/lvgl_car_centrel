#ifndef TIMEPANEL_H
#define TIMEPANEL_H

#include"lvgl/lvgl.h"

typedef struct 
{
    char date[16];
    char wea[32];
    char tem1[8];
    char tem2[8];
}DayWeather;

typedef struct{
    char city[32];
    DayWeather days[3];
}WealthInfo;

int fetch_weather_date(const char *city_name , WealthInfo *out_info);
void update_weather_ui(const WealthInfo *info);
void* weather_thread_func(void *arg);       //线程获取天气函数

#endif








