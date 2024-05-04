/**
 * Header file which contains wifi related utilities.
 * 
 * Author: Kush Bharakhada and Alex Chapman (2024)
 * Filename: wifi_utils.h
*/

#ifndef WIFI_UTILS_H
#define WIFI_UTILS_H

#include <Arduino.h>
#include <WiFi.h>
#include <private.h>

/**
 * Attempt to reconnect to the WiFi if there is a disconnection.
 * Uses RTOS for threading.
 * 
 * @param params - Parameters passed by the thread into the function, NOT USED.
 */
void keepWiFiAlive(void *params);

class ESPWiFi {
    public:
        // Constants describing the WiFi connection stability
        const static uint8_t NO_CONNECTION = 0;
        const static uint8_t WEAK_CONNECTION = 1;
        const static uint8_t MODERATE_CONNECTION = 2;
        const static uint8_t STRONG_CONNECTION = 3;

        /**
         * Constructor for the WiFi utils.
         */
        ESPWiFi();

        /**
         * Initialises the WiFi connection.
         */
        void begin();

        /**
         * Checks the unPhoneWiFi status.
         * 
         * @return - Boolean of the unPhoneWiFi status.
         */
        bool isConnected();

        /**
         * Retrieves the strength of the WiFi connection.
         * 
         * @return - Integer {0, 1, 2, 3} based on the WiFi strength or no-connection.
         */
        uint8_t getWiFiStrength();
};

#endif