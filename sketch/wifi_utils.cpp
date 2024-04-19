/**
 * Contains wifi related utilities
 * 
 * Author: Kush Bharakhada and Alex Chapman
 * wifi_utils.cpp
*/

#include <wifi_utils.h>

const long TIMEOUT_MS = 10000;

ESPWiFi::ESPWiFi(){}

void ESPWiFi::begin() {
    xTaskCreatePinnedToCore(
        keepWiFiAlive,
        "Keeps WiFi alive",
        5000,
        NULL,
        1,
        NULL,
        CONFIG_ARDUINO_RUNNING_CORE
    );
};

bool ESPWiFi::isConnected() {
    return WiFi.status() == WL_CONNECTED;
};

uint8_t ESPWiFi::getWiFiStrength() {
    // If not connected then 0 connection
    if (!isConnected()) {
        return 0;
    } 

    long GOOD_RSS = -40;
    long OK_RSS = -70;
    long POOR_RSS = 0;
    long rss = WiFi.RSSI();
    
    // The better the connection the higher the value
    if (rss > GOOD_RSS) {
        return 3;
    } else if (rss > OK_RSS) {
        return 2;
    } else {
        return 1;
    }
}


void keepWiFiAlive(void * params) {
    while(true) {
        if (WiFi.status() == WL_CONNECTED) {
            vTaskDelay(10000 / portTICK_PERIOD_MS);
            continue;
        }
        
        WiFi.begin(WIFI_SSID, WIFI_PASSWORD); // Connect to wifi
        
        unsigned long startTime = millis();
        
        while (WiFi.status() != WL_CONNECTED && (millis() - startTime < TIMEOUT_MS)) {
            vTaskDelay(1000 / portTICK_PERIOD_MS);
        } // Wait until wifi is connected

        if (WiFi.status() != WL_CONNECTED) {
            vTaskDelay(10000 / portTICK_PERIOD_MS);
            continue;
        }
    }
};
