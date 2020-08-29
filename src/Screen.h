//
// Created by mano233 on 2020/8/25.
//
#pragma once
#ifndef TTGO_T5_V2_3_CLOCK_SCREEN_H
#define TTGO_T5_V2_3_CLOCK_SCREEN_H

#include <U8g2_for_Adafruit_GFX.h>
#include <Adafruit_GFX.h>
#include <Fonts/FreeSans9pt7b.h>
#include <Fonts/FreeSansBoldOblique18pt7b.h>
#include <Fonts/FreeSerifBold9pt7b.h>
#include <GxEPD.h>
#include <GxIO/GxIO_SPI/GxIO_SPI.h>
#include <GxIO/GxIO.h>
#include "type.h"
#include "text.h"
#include "ctime"
#include "board_def.h"


#define ICONBITMAPSIZE 100

class Screen {
private:
    GxEPD_Class *display;
    GxIO_Class *gxIo;
    U8G2_FOR_ADAFRUIT_GFX *u8g2;
public:
    explicit Screen(SPIClass &spi);

    ~Screen();

    void init();

    void powerOff();

    void update();

    void showMainPage();

    void showMsgPage();

    void showErrorPage();

    void drawTime(uint8_t hour, uint8_t min);

    void drawBme(const bme_t *bmeData);

    void drawWeather(int x, int y, const weather_t *weatherData);
private:
    void LCD_one(int num, int loc);
    // 解析天气json中的天气code，返回对应icon 的bitmap的起始位置
    static inline void weatherIconDecoder(int weatherCode, int *startIndex) {
        if (weatherCode >= 0 && weatherCode <= 38) {
            *startIndex = weatherCode * ICONBITMAPSIZE;
        } else if (weatherCode == 99) {
            *startIndex = 39 * ICONBITMAPSIZE;
        }
    }

};

#endif //TTGO_T5_V2_3_CLOCK_SCREEN_H
