#include "Borad.h"
#include <NTPClient.h>
#include <Wire.h>

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






