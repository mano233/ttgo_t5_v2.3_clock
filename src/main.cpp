#include "Board.h"
#include <WiFi.h>
#include <ctime>

#include <string>
#include <Ticker.h>

#define WIFI_SSID "***"
#define WIFI_PASSWORD "****"
#define GMTOFFSET_SEC 3600*7
#define NTP_SERVER "pool.ntp.org"
#define DAYLIGHTOFFSET_SEC 3600


bool hasError = false;

void update();

int updateCount = 0;

Board board;

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
        Serial.print('.');
    }
    Serial.flush();
    Serial.printf("WiFi connected:%s\n\r", WiFi.localIP().toString().c_str());
    // 配置ntp时间设置
    configTime(GMTOFFSET_SEC, DAYLIGHTOFFSET_SEC, NTP_SERVER);

    board.init();

    ticker.start(); //start the ticker.
    setCpuFrequencyMhz(80);
    Serial.printf("CPU set frequency: %u mhz\n", getCpuFrequencyMhz());
    WiFi.setSleep(true);
    if (WiFi.getSleep()) {
        Serial.println("wifi sleep...");
    }
    esp_sleep_enable_timer_wakeup(1000000*30);
    esp_light_sleep_start();
}

void update() {
    Serial.printf("update....[%d]\n", updateCount++);
    struct tm timeinfo{};
    // 从ntp获取时间
    if (!getLocalTime(&timeinfo, 50000)) {
        Serial.println("Failed to obtain time");
        hasError = true;
        return;
    }
    // board.screen->drawTime(timeinfo.tm_hour, timeinfo.tm_min);
    board.updateBmeData();
    // 每小时更新一次天气
    if (updateCount >= 60) {
        updateCount = 0;
        // board.updateWeather();
    }
    esp_sleep_enable_timer_wakeup(1000000);
    esp_light_sleep_start();
}


void loop() {
    ticker.update();
    if (hasError) {
       board.errorHandler();
    }
}
