//
// Created by mano233 on 2020/8/27.
//

#ifndef TTGO_T5_V2_3_CLOCK_TYPE_H
#define TTGO_T5_V2_3_CLOCK_TYPE_H
#pragma once

struct weather_t{
    char weatherStr[30];
    int weatherCode;
    int tempH;
    int tempL;
};

typedef struct {
    int humidity;
    int pressure;
    int temperature;
    int iaq;
} bme_t;
#endif //TTGO_T5_V2_3_CLOCK_TYPE_H
