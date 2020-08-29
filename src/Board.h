//
// Created by mano233 on 2020/8/25.
//

#ifndef TTGO_T5_V2_3_CLOCK_BOARD_H
#define TTGO_T5_V2_3_CLOCK_BOARD_H

#include "board_def.h"
#include "Screen.h"
#include <SPI.h>
#include <WiFi.h>
#include <esp_adc_cal.h>
#include <Adafruit_BME280.h>
#include <Button2.h>
#include <Wire.h>
#include <HTTPClient.h>
#include <ArduinoJson.h>



class Board {
private:
    SPIClass spi;
    Adafruit_BME280 bme;
    Button2 button = Button2(BUTTON_3);
    bme_t bme280_info{0, 0, 0};
    void buttonInit();
public:
    Screen *screen = nullptr;

    void errorHandler() const;

    void init();

    Board();

    void updateBmeData();

    void updateWeather() const;

    ~Board();
};

#endif //TTGO_T5_V2_3_CLOCK_BOARD_H
