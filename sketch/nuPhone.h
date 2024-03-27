/**
 * A reduced version of unPhone.h
 * 
 * Author: Kush Bharakhada and Alex Chapman
 * NuPhone.h
*/

#ifndef NUPHONE_H
#define NUPHONE_H

#include <stdint.h>
#include <Arduino.h>
#include <esp_sleep.h> // sleep on powerswitch
#include <esp32-hal-gpio.h> // read gpio pins
#include <WiFi.h> // manage WiFi connections
#include <XPT2046_Touchscreen.h>
#include <Wire.h>
#include <TCA9555.h>
// #include <Adafruit_HX8357.h> 

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ NUPHONE CLASS ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

class NuPhone {
    
    public:
        NuPhone();
        static NuPhone *me;
        XPT2046_Touchscreen *tsp;

        void begin();
        void checkPowerSwitch();
        void setLEDRGB(uint8_t red, uint8_t green, uint8_t blue);
        void setBacklight(bool shouldBacklight);
    
    private:
        void initializeTCAChip();
};

#endif