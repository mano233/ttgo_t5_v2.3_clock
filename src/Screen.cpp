//
// Created by mano233 on 2020/8/25.
//
#include "Screen.h"



void Screen::init() {
    Serial.println("---screen init---");
    display->init();
    u8g2->begin(*display);
    display->eraseDisplay();
    display->setRotation(3);
}

Screen::Screen(SPIClass &spi) {
    gxIo = new GxIO_Class(spi, ELINK_SS, ELINK_DC, ELINK_RESET);
    display = new GxEPD_Class(*gxIo, ELINK_RESET, ELINK_BUSY);
    u8g2 = new U8G2_FOR_ADAFRUIT_GFX();
}

Screen::~Screen() {
    delete display;
    delete u8g2;
}

void Screen::update() {
    Serial.println("---screen full update---");
    display->update();
}

void Screen::showMainPage() {
    drawTime(0, 0);
    bme_t initData={0,0,0,0};
    drawBme(&initData);
    struct tm timeinfo{};
    if (!getLocalTime(&timeinfo, 50000)) {
        Serial.println("Failed to obtain time");
        return;
    }
    drawTime(timeinfo.tm_hour, timeinfo.tm_min);
}

void Screen::showMsgPage() {

}

void Screen::showErrorPage() {
    display->eraseDisplay();
    display->fillScreen(GxEPD_WHITE);
    update();
    u8g2->setForegroundColor(GxEPD_BLACK);
    u8g2->setBackgroundColor(GxEPD_WHITE);
    u8g2->setFont(u8g2_font_calibration_gothic_nbp_tf);
    u8g2->setCursor(70, 55);
    u8g2->print("An error occurred.");
    u8g2->setCursor(70, 85);
    u8g2->print("Please  restart");
    display->drawBitmap(10, 38, oops, 50, 50, GxEPD_BLACK);
    update();
}

void Screen::LCD_one(int code, int loc) {
    uint16_t show_1, show_2, show_3, show_4, show_5, show_6, show_7;
    int x;
    switch (loc) {
        case 1:
            x = 0 - 9;
            break;
        case 2:
            x = 34 - 9;
            break;
        case 3:
            x = 77 - 9;
            break;
        case 4:
            x = 111 - 9;
            break;
        default:
            break;
    }
    if (code == 4 || code == 5 || code == 6 || code == 8 || code == 9 || code == 0)
        show_1 = GxEPD_BLACK;
    else show_1 = GxEPD_WHITE;
    if (code == 2 || code == 6 || code == 8 || code == 0)show_2 = GxEPD_BLACK; else show_2 = GxEPD_WHITE;
    if (code == 1 || code == 2 || code == 3 || code == 4 || code == 7 || code == 8 || code == 9 || code == 0)
        show_3 = GxEPD_BLACK;
    else show_3 = GxEPD_WHITE;
    if (code == 1 || code == 3 || code == 4 || code == 5 || code == 6 || code == 7 || code == 8 || code == 9 ||
        code == 0)
        show_4 = GxEPD_BLACK;
    else show_4 = GxEPD_WHITE;
    if (code == 2 || code == 3 || code == 5 || code == 6 || code == 7 || code == 8 || code == 9 || code == 0)
        show_5 = GxEPD_BLACK;
    else show_5 = GxEPD_WHITE;
    if (code == 2 || code == 3 || code == 4 || code == 5 || code == 6 || code == 8 || code == 9)
        show_6 = GxEPD_BLACK;
    else show_6 = GxEPD_WHITE;
    if (code == 2 || code == 3 || code == 5 || code == 6 || code == 8 || code == 9 || code == 0)
        show_7 = GxEPD_BLACK;
    else show_7 = GxEPD_WHITE;
    display->drawBitmap(115 + x, 20, LCD_l, 5, 24, show_1);//数码管
    display->drawBitmap(115 + x, 47, LCD_l, 5, 24, show_2);//数码管
    display->drawBitmap(140 + x, 20, LCD_l, 5, 24, show_3);//数码管
    display->drawBitmap(140 + x, 47, LCD_l, 5, 24, show_4);//数码管
    display->drawBitmap(119 + x, 16, LCD_h, 22, 5, show_5);//数码管
    display->drawBitmap(119 + x, 43, LCD_h, 22, 5, show_6);//数码管
    display->drawBitmap(119 + x, 70, LCD_h, 22, 5, show_7);//数码管
}

void Screen::drawTime(uint8_t hour, uint8_t min) {
    LCD_one(hour / 10, 1);
    LCD_one(hour % 10, 2);
    LCD_one(min / 10, 3);
    LCD_one(min % 10, 4);
    // 画两个点，：
    display->drawBitmap(183 - 9, 31, LCD_d, 5, 5, GxEPD_BLACK);
    display->drawBitmap(183 - 9, 55, LCD_d, 5, 5, GxEPD_BLACK);

    display->updateWindow(114 - 9, 16, 141, 59, true);
}

void Screen::drawBme(const bme_t *bmeData) {
    display->fillRect(0, 0, 49, 63-8, GxEPD_BLACK);
    u8g2->setForegroundColor(GxEPD_WHITE);
    u8g2->setFont(u8g2_font_helvB24_tn);
    u8g2->setCursor(4, 32);
    u8g2->print(bmeData->temperature / 10);

    u8g2->setForegroundColor(GxEPD_WHITE);
    u8g2->setFont(u8g2_font_7x14B_tf);
    u8g2->setCursor(41, 31);
    u8g2->print(bmeData->temperature % 10);

    display->fillRect(52, 0, 48, 26, GxEPD_BLACK);
    u8g2->setForegroundColor(GxEPD_WHITE);
    u8g2->setFont(u8g2_font_lastapprenticebold_tr);
    u8g2->setCursor(74, 20);
    display->drawBitmap(53, 4, p_icon, 18, 18,GxEPD_WHITE);
    u8g2->print(bmeData->pressure);

    display->fillRect(52, 29, 48, 26, GxEPD_BLACK);
    display->drawBitmap(53, 33, h_icon, 18, 18,GxEPD_WHITE);
    u8g2->setForegroundColor(GxEPD_WHITE);
    u8g2->setCursor(74, 48);
    u8g2->print(String(bmeData->humidity)+"%");
    display->updateWindow(0,0,100,64);
}

void Screen::powerOff() {
    display->powerDown();
}

void Screen::drawWeather(int x, int y, const char *weatherStr, int weatherCode,int tempL, int tempH) {
    int w = 100;
    int h = 30;
    int iconSize = 25;
    display->fillRect(x, y, w, h, GxEPD_BLACK);

    int startIndex =0 ;
    weatherIconDecoder(weatherCode,&startIndex);
    Serial.printf("weatherCode:%d...\n",weatherCode);
    unsigned char icon[100]={0};
    memcpy(icon,WEATHERICON+startIndex,ICONBITMAPSIZE);
    // 绘制天气icon
    display->drawBitmap(x + 3, y + 3, icon, iconSize, iconSize, GxEPD_WHITE);
    u8g2->setFont(u8g2_font_wqy13_t_gb2312b);
    u8g2->setFontDirection(0);
    u8g2->setFontMode(1);
    u8g2->setForegroundColor(GxEPD_WHITE);
    u8g2->setForegroundColor(GxEPD_WHITE);
    u8g2->setCursor(33 + x, y + 14);
    // 绘制天气文本
    u8g2->print(weatherStr);

    for (int i = 0; i < 2; i++) {
        int temp = i == 0 ? tempL : tempH;
        int num_l, num_h;
        num_l = abs(temp) % 10;
        num_h = abs(temp) / 10;
        int marginX = i == 0 ? 30 : 70;
        int marginY = 14;
        int fontW = 8;
        int fontH = 16;
        if (temp > 0) {
            display->drawBitmap(x + marginX, y + marginY, num[num_h], fontW, fontH, GxEPD_WHITE);//十位
            display->drawBitmap(x + marginX + fontW, y + marginY, num[num_l], fontW, fontH, GxEPD_WHITE);//个位
            display->drawBitmap(x + marginX + fontW * 2, y + marginY, num[10], fontW, fontH, GxEPD_WHITE);//℃
        } else if (temp == 0) {
            display->drawBitmap(x + marginX + fontW, y + marginY, num[num_l], fontW, fontH, GxEPD_WHITE);//个位
            display->drawBitmap(x + marginX + fontW * 2, y + marginY, num[10], fontW, fontH, GxEPD_WHITE);//℃
        } else if (temp < 0) {
            if (temp < -9) {
                display->drawBitmap(x + marginX, y + marginY, num[11], fontW, fontH, GxEPD_WHITE);//-
                display->drawBitmap(x + marginX + fontW, y + marginY, num[num_h], fontW, fontH, GxEPD_WHITE);//十位
                display->drawBitmap(x + marginX + fontW * 2, y + marginY, num[num_l], fontW, fontH, GxEPD_WHITE);//个位
            } else {
                display->drawBitmap(x + marginX, y + marginY, num[11], fontW, fontH, GxEPD_WHITE);//-
                display->drawBitmap(x + marginX + fontW, y + marginY, num[num_l], fontW, fontH, GxEPD_WHITE);//个位
                display->drawBitmap(x + marginX + fontW * 2, y + marginY, num[10], fontW, fontH, GxEPD_WHITE);//℃
            }
        }
        display->drawBitmap(x + 54, y + 15, num_, 16, 16, GxEPD_WHITE);//--

        display->updateWindow(0, 58, 100, 64, true);
    }
}

