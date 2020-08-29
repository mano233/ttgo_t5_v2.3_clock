#include "Borad.h"
#include <NTPClient.h>
#include <Wire.h>
#include <HTTPClient.h>
#include <ArduinoJson.h>
#include <WiFi.h>
#include <Adafruit_BME280.h>

HTTPClient *http;

struct weather_t weatherInfo;

Board::Board() {
    spi = new SPIClass();
    screen = new Screen(*spi);
    bme = new Adafruit_BME280();
}


void Board::init() {
    Serial.println("---board init---");
    // spi->begin(SPI_CLK, SPI_MISO, SPI_MOSI, -1);
    if(!bme->begin(0x76,&Wire)){
        Serial.println("bme not found!");
    }
    updateBmeData();
    // Serial.printf("bme_info:{ temperature:%d,humidity:%d,pressure:%d }",(int)bme->readTemperature(),(int)bme->readHumidity(),(int)(bme->readPressure() / 100.0F));
    screen->init();
    screen->showMainPage();
    updateBmeData();
    updateWeather();
}

Board::~Board() {
    delete screen;
    delete spi;
}

void Board::updateBmeData() {
    Serial.println("---update bme280 data---");
    bme280_info.temperature= (int)bme->readTemperature()*10;
    bme280_info.humidity= (int)bme->readHumidity();
    bme280_info.pressure= (int)(bme->readPressure() / 100.0F);
    Serial.printf("bme_info:{ temperature:%d,humidity:%d,pressure:%d }\n",bme280_info.temperature,bme280_info.humidity,bme280_info.pressure);
    screen->drawBme(&bme280_info);
}

void Board::errorHandler() const {
    Serial.flush();
    screen->showErrorPage();
    screen->powerOff();
    esp_deep_sleep_start();
}

void Board::updateWeather() {
    StaticJsonDocument<3000> jsonBuffer;
    http->end();
    http->begin(
            "https://api.seniverse.com/v3/weather/daily.json?key=S9ecTZM2j06C6HWR3&location=WTMKQ069CCJ7&language=zh-Hans&unit=c&start=0&days=2");
    int httpResponseCode = http->GET();
    if (httpResponseCode > 0) {
        DeserializationError error = deserializeJson(jsonBuffer, http->getString());
        // Test if parsing succeeds.
        if (error) {
            Serial.print(F("deserializeJson() failed: "));
            Serial.println(error.c_str());
        }
        struct weather_t todayWeather = {
                .weatherStr  = {jsonBuffer["results"][0]["daily"][0]["text_day"]},
                .weatherCode = atoi(jsonBuffer["results"][0]["daily"][0]["code_day"]),
                .tempH = atoi(jsonBuffer["results"][0]["daily"][0]["high"]),
                .tempL = atoi(jsonBuffer["results"][0]["daily"][0]["low"])
        };
        struct weather_t tomorrowWeather = {
                .weatherStr  = {jsonBuffer["results"][0]["daily"][1]["text_day"]},
                .weatherCode = atoi(jsonBuffer["results"][0]["daily"][1]["code_day"]),
                .tempH = atoi(jsonBuffer["results"][0]["daily"][1]["high"]),
                .tempL = atoi(jsonBuffer["results"][0]["daily"][1]["low"])
        };

        Serial.printf("今日天气：%s\n\r", todayWeather.weatherStr);
        Serial.printf("今日最高气温：%d\n\r", todayWeather.tempH);
        Serial.printf("今日最低气温：%d\n\r", todayWeather.tempL);
        Serial.printf("明日天气：%s\n\r", tomorrowWeather.weatherStr);
        Serial.printf("明日最高气温：%d\n\r", tomorrowWeather.tempH);
        Serial.printf("明日最低气温：%d\n\r", tomorrowWeather.tempL);

        screen->drawWeather(0, 59, &todayWeather);
        screen->drawWeather(0, 92, &tomorrowWeather);
    }


    http->end();
}







