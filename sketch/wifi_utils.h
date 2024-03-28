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

class ESPWiFi {
    public:
        ESPWiFi();
        void begin();
};

#endif