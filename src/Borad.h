//
// Created by mano233 on 2020/8/25.
//

#ifndef TTGO_T5_V2_3_CLOCK_BORAD_H
#define TTGO_T5_V2_3_CLOCK_BORAD_H
#include <esp_adc_cal.h>

#include "board_def.h"
#include "Screen.h"
#include <SPI.h>
#include "board_def.h"
#include <WiFi.h>
#include <Adafruit_BME280.h>

class Board {
private:
    SPIClass *spi= nullptr;
    Adafruit_BME280 *bme= nullptr;
public:
    Screen *screen= nullptr;
    void errorHandler() const;
    bme_t bme280_info{0,0,0};
    Board();
    void updateBmeData();
    void updateWeather();
    ~Board();
    void init();
};


#endif //TTGO_T5_V2_3_CLOCK_BORAD_H
