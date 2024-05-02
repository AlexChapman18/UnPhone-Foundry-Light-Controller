/**
 * Contains Artnet related utilities.
 * 
 * Author: Kush Bharakhada and Alex Chapman (2024)
 * Filename: artnet_utils.h
*/

#ifndef ARTNET_UTILS_H
#define ARTNET_UTILS_H

#include <Arduino.h>
#include <stdint.h>
#include <ArtnetWifi.h>
#include <private.h>
#include <fixture_utils.h>

/**
 * Repeatedly calculates and sends the universe, used in seperate thread.
 *
 * @param params - Parameters passed by the thread into the function, NOT USED.
 */
void keepSendingUniverse(void *params);

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ ARTNET UNIVERSE CLASS ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

class ArtNetUniverse {
    public:
        // Class variables
        static uint8_t color_universe[512];
        static uint8_t current_effect;
        static float current_speed;
        static float current_intensity;

        /**
         * Constructor for Artnet universe object.
         */
        ArtNetUniverse();

        /**
         * Initialises the current values and configures the Artnet connection.
         */
        void setup();

        /**
         * Creates the output thread for Artnet packets.
         */
        void begin();

        /**
         * Setter for the current intensity.
         * 
         * @param _intensity - New intensity.
         */     
        void setIntensity(float _intensity);

        /**
         * Setter for the current speed.
         * 
         * @param _speed - New speed.
         */    
        void setSpeed(float _speed);

        /**
         * Getter for the current intensity value.
         * 
         * @return current_intensity - Current intensity,
         */    
        float getIntensity();

        /**
         * Getter for the current speed value.
         * 
         * @return current_speed - Current speed.
         */  
        float getSpeed();
};

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ EFFECTS ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

/**
 * Calculate the universe for effect 1.
 * EFFECT 1, SOLID - No changes in brightness.
 * 
 * @param artnet - Artnet universe object.
 */  
void effect1(ArtnetWifi *artnet);

/**
 * Calculate the universe for effect 2.
 * EFFECT 2, PULSE - All of the LED's on and off at the same time.
 * 
 * @param artnet - Artnet universe object.
 * @param step   - current step in the effect.
 */  
void effect2(ArtnetWifi *artnet, float step);

/**
 * Calculate the universe for effect 3.
 * EFFECT 3, ODD-EVEN - Alternatingly dims the odd and even indexed LED fixtures over time.
 * 
 * @param artnet - Artnet universe object.
 * @param step   - current step in the effect.
 */  
void effect3(ArtnetWifi *artnet, float step);

/**
 * Calculate the universe for effect 4.
 * EFFECT 4, FADE-SWIPE - Has an intensity value go round the room leaving a decaying brightness train.
 * 
 * @param artnet - Artnet universe object.
 * @param step   - current step in the effect.
 */  
void effect4(ArtnetWifi *artnet, float step);

/**
 * Calculate the universe for effect 5.
 * EFFECT 5, BINARY-SWIPE - Same as the fade swipe, but the intensity is either 255 or 0.
 * 
 * @param artnet - Artnet universe object.
 * @param step   - current step in the effect.
 */  
void effect5(ArtnetWifi *artnet, float step);

/**
 * Calculate the universe for effect 6.
 * EFFECT 6, BARS-FADE - Applies a rolling fade effect but only over the bar arcs.
 * 
 * @param artnet - Artnet universe object.
 * @param step   - current step in the effect.
 */  
void effect6(ArtnetWifi *artnet, float step);

#endif