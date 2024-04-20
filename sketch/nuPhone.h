/**
 * A reduced version of unPhone.h
 * 
 * Author: Kush Bharakhada and Alex Chapman (2024)
 * Filename: LVGL_utils.h
*/

#ifndef NUPHONE_H
#define NUPHONE_H

#include <stdint.h>
#include <Arduino.h>
#include <esp_sleep.h>
#include <esp32-hal-gpio.h>
#include <WiFi.h>
#include <XPT2046_Touchscreen.h>
#include <Wire.h>
#include <TCA9555.h>

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ NUPHONE CLASS ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

class NuPhone { 
    private:
        // Private class variables
        bool isButton1Held;
        bool isButton2Held;
        bool isButton3Held;

        /**
         * Initialise the TCA chip
        */
        void initializeTCAChip();

    public:
        // Public class variables
        static NuPhone *me;
        XPT2046_Touchscreen *tsp;

        /**
         * Constructor
         */
        NuPhone();

        /**
         * Sets the pins, initialises the TCA chip, sets the touchscreen and sets up the
         * power checking function for the switch
         */
        void begin();
        
        /**
         * Checks the state of the power switch and runs the appropriate functions
         */
        void checkPowerSwitch();

        /**
         * Sets the state of the backlight
         * 
         * @param shouldBacklight  - should the backlight be on or off
         */
        void setBacklight(bool shouldBacklight);

        /**
         * gets the current status of button 1
         * 
         * @return bool  - is button 1 pressed
         */
        bool isButton1();

        /**
         * gets the current status of button 2
         * 
         * @return bool  - is button 2 pressed
         */
        bool isButton2();

        /**
         * gets the current status of button 3
         * 
         * @return bool  - is button 3 pressed
         */
        bool isButton3();
};

#endif