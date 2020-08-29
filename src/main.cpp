#include "Borad.h"
#include <WiFi.h>
#include "ctime"
#include <HTTPClient.h>
#include <ArduinoJson.h>
#include <string>
#include <Ticker.h>
#include "type.h"


#define WIFI_SSID "***"
#define WIFI_PASSWORD "****"
#define GMTOFFSET_SEC 3600*7
#define NTP_SERVER "pool.ntp.org"
#define DAYLIGHTOFFSET_SEC 3600

struct weather_t weatherInfo;

HTTPClient *http;

bool hasError = false;

void updateWeather();

struct tm lastTime{};

void update();

int updateCount = 0;

Board *board = nullptr;

// 1分钟触发一次
Ticker ticker(update, 1000 * 60);


void setup() {
    Serial.begin(115200);
    WiFiClass::mode(WIFI_STA);
    WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
    Serial.printf("CPU get frequency: %u mhz\n", getCpuFrequencyMhz());
    Serial.println("init wifi");
    while (WiFi.waitForConnectResult() != WL_CONNECTED) {
        delay(100);
    }
    http = new HTTPClient();


    Serial.printf("WiFi connected:%s\n\r", WiFi.localIP().toString().c_str());
    // 配置ntp时间设置
    configTime(GMTOFFSET_SEC, DAYLIGHTOFFSET_SEC, NTP_SERVER);
    board = new Board();
    board->init();

    updateWeather();
    board->screen->drawWeather(0, 59, weatherInfo.weatherToday, weatherInfo.weatherTodayCode,weatherInfo.tempTodayL, weatherInfo.tempTodayH);
    board->screen->drawWeather(0, 92, weatherInfo.weatherTomorrow, weatherInfo.weatherTomorrowCode, weatherInfo.tempTodayL,weatherInfo.tempTomorrowH);

    ticker.start(); //start the ticker.
    setCpuFrequencyMhz(80);
    Serial.printf("CPU set frequency: %u mhz\n", getCpuFrequencyMhz());
    WiFi.setSleep(true);
    if (WiFi.getSleep()) {
        Serial.println("wifi sleep...");
    }
}

void update() {
    Serial.printf("update....[%d]\n\r", updateCount++);
    struct tm timeinfo{};
    // 从ntp获取时间
    if (!getLocalTime(&timeinfo, 50000)) {
        Serial.println("Failed to obtain time");
        hasError = true;
        return;
    }
    board->screen->drawTime(timeinfo.tm_hour, timeinfo.tm_min);
    board->updateBmeData();
    // 每小时更新一次天气
    if (updateCount >= 60) {
        updateCount = 0;
        Serial.printf("update weather...\n\r");
        updateWeather();
    }
}

void updateWeather() {
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
        strcpy(weatherInfo.weatherToday, jsonBuffer["results"][0]["daily"][0]["text_day"]);
        strcpy(weatherInfo.weatherTomorrow, jsonBuffer["results"][0]["daily"][1]["text_day"]);

        weatherInfo.weatherTodayCode = atoi(jsonBuffer["results"][0]["daily"][0]["code_day"]);
        weatherInfo.weatherTomorrowCode = atoi(jsonBuffer["results"][0]["daily"][1]["code_day"]);

        weatherInfo.tempTodayH = atoi(jsonBuffer["results"][0]["daily"][0]["high"]);
        weatherInfo.tempTodayL = atoi(jsonBuffer["results"][0]["daily"][0]["low"]);
        weatherInfo.tempTomorrowH = atoi(jsonBuffer["results"][0]["daily"][1]["high"]);
        weatherInfo.tempTomorrowL = atoi(jsonBuffer["results"][0]["daily"][1]["low"]);
        Serial.printf("今日天气：%s\n\r", weatherInfo.weatherToday);
        Serial.printf("今日最高气温：%d\n\r", weatherInfo.tempTodayH);
        Serial.printf("今日最低气温：%d\n\r", weatherInfo.tempTodayL);
        Serial.printf("明日天气：%s\n\r", weatherInfo.weatherTomorrow);
        Serial.printf("明日最高气温：%d\n\r", weatherInfo.tempTomorrowH);
        Serial.printf("明日最低气温：%d\n\r", weatherInfo.tempTomorrowL);
    }
    http->end();
}

void loop() {
    ticker.update();
    if (hasError) {
       board->errorHandler();
    }
}
