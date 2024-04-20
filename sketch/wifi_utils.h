/**
 * Contains wifi related utilities
 * 
 * Author: Kush Bharakhada and Alex Chapman
 * wifi_utils.h
*/

#ifndef WIFI_UTILS_H
#define WIFI_UTILS_H

#include <Arduino.h>
#include <WiFi.h>
#include <private.h>

void keepWiFiAlive(void * params);

class ESPWiFi {
    public:
        const static uint8_t NO_CONNECTION = 0;
        const static uint8_t WEAK_CONNECTION = 1;
        const static uint8_t MODERATE_CONNECTION = 2;
        const static uint8_t STRONG_CONNECTION = 3;

        ESPWiFi();
        void begin();
        bool isConnected();
        uint8_t getWiFiStrength();
};

#endif