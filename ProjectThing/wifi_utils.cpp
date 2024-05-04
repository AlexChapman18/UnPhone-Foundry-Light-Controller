/**
 * Contains wifi related utilities.
 * 
 * Author: Kush Bharakhada and Alex Chapman (2024)
 * Filename: wifi_utils.cpp
*/

#include <wifi_utils.h>

// Determines the interval for WiFi reconnection attempts
const long TIMEOUT_MS = 10000;

void keepWiFiAlive(void *params) {
    while (true) {
        // Check if WiFi is connected, if it is then do nothing
        if (WiFi.status() == WL_CONNECTED) {
            vTaskDelay(10000 / portTICK_PERIOD_MS);
            continue;
        }
        
        // If WiFi is not connected then attempt to connect
        WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
        
        // Waiting until connected
        unsigned long startTime = millis();
        while (WiFi.status() != WL_CONNECTED && (millis() - startTime < TIMEOUT_MS)) {
            vTaskDelay(1000 / portTICK_PERIOD_MS);
        }
    }
};

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ ESP WIFI CLASS ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

// Constructor
ESPWiFi::ESPWiFi(){}

void ESPWiFi::begin() {
    // Creates a new thread on the same core that the Arduino is running on
    // and runs keep WiFi alive on that core
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
    // Check WiFi status
    return WiFi.status() == WL_CONNECTED;
};

uint8_t ESPWiFi::getWiFiStrength() {
    // If not connected then 0 connection
    if (!isConnected()) {
        return 0;
    } 

    // Connection strength values
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
