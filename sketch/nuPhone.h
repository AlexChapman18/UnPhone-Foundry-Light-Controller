/**
 * A reduced version of unPhone.h.
 * 
 * Author: Kush Bharakhada and Alex Chapman (2024)
 * Filename: nuPhone.h
*/

#ifndef NUPHONE_H
#define NUPHONE_H

#include <stdint.h>
#include <Arduino.h>
#include <XPT2046_Touchscreen.h>
#include <Wire.h>

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ NUPHONE CLASS ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

class NuPhone { 
    private:
        // Private class variables
        bool isButton1Held;
        bool isButton2Held;
        bool isButton3Held;

        /**
         * Initialise the TCA chip.
         */
        void initializeTCAChip();

    public:
        // Public class variables
        static NuPhone *me;
        XPT2046_Touchscreen *tsp;

        /**
         * Constructor.
         */
        NuPhone();

        /**
         * Sets the pins, initialises the TCA chip, and sets up the touchscreen and the
         * power checking function for the switch.
         */
        void begin();
        
        /**
         * Checks the state of the power switch and runs the appropriate functions.
         * Turn the device off if necessary.
         */
        void checkPowerSwitch();

        /**
         * Sets the state of the backlight.
         * 
         * @param shouldBacklight - Whether the backlight be on or off.
         */
        void setBacklight(bool shouldBacklight);

        /**
         * Gets the current status of button 1.
         * 
         * @return - Boolean whether button 1 is pressed.
         */
        bool isButton1();

        /**
         * Gets the current status of button 2.
         * 
         * @return - Boolean whether button 2 is pressed.
         */
        bool isButton2();

        /**
         * Gets the current status of button 3.
         * 
         * @return - Boolean whether button 3 is pressed.
         */
        bool isButton3();
};

#endif