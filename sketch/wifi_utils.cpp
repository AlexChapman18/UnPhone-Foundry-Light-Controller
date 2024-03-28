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


void keepWiFiAlive(void * params) {
    while(true) {
        if (WiFi.status() == WL_CONNECTED) {
            vTaskDelay(10000 / portTICK_PERIOD_MS);
            continue;
        }
        
        WiFi.begin(WIFI_SSID, WIFI_PASSWORD); // Connect to wifi
        
        unsigned long startTime = millis();
        
        while (WiFi.status() != WL_CONNECTED && (millis() - startTime < TIMEOUT_MS)) {} // Wait until wifi is connected

        if (WiFi.status() != WL_CONNECTED) {
            vTaskDelay(10000 / portTICK_PERIOD_MS);
            continue;
        }
    }
};
