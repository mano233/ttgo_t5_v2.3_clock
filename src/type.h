//
// Created by mano233 on 2020/8/27.
//

#ifndef TTGO_T5_V2_3_CLOCK_TYPE_H
#define TTGO_T5_V2_3_CLOCK_TYPE_H
#pragma once
struct weather_t{
    char weatherToday[30]={};
    int weatherTodayCode=99;
    int weatherTomorrowCode=99;
    char weatherTomorrow[30]={};
    int tempTodayH = 0;
    int tempTodayL = 0;
    int tempTomorrowH = 0;
    int tempTomorrowL = 0;
};
typedef struct {
    int humidity;
    int pressure;
    int temperature;
    int iaq;
} bme_t;
#endif //TTGO_T5_V2_3_CLOCK_TYPE_H
