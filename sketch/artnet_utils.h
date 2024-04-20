/**
 * Contains art-net related utilities
 * 
 * Author: Kush Bharakhada and Alex Chapman (2024)
 * Filename: fixture_utils.cpp
*/

#ifndef ARTNET_UTILS_H
#define ARTNET_UTILS_H

#include <Arduino.h>
#include <stdint.h>
#include <ArtnetWifi.h>
#include <private.h>
#include <fixture_utils.h>


/**
 * Repeadly calculates and sends the universe, used in seperate thread
 *
 * @param params  - Parameters passed by the thread into the function, NOT USED
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
         * Constructor for art-net universe object
         */
        ArtNetUniverse();


        /**
         * Initialises the current values and configures the art-net connection
         */
        void setup();

        /**
         * Creates the output thread for art-net packets
         */
        void begin();


        /**
         * Setter for the current intensity
         * 
         * @param _intensity  - new intensity
         */     
        void setIntensity(float _intensity);

        /**
         * Setter for the current speed
         * 
         * @param _speed  - new speed
         */    
        void setSpeed(float _speed);


        /**
         * Getter for the current intensity value
         * 
         * @return current_intensity  - current intensity 
         */    
        float getIntensity();

        /**
         * Getter for the current speed value
         * 
         * @return current_speed  - current speed 
         */  
        float getSpeed();
};


/**
 * Calculate the universe for effect 1
 * 
 * @param artnet  - art-net universe object
 */  
void effect1(ArtnetWifi* artnet);


/**
 * Calculate the universe for effect 2
 * 
 * @param artnet  - art-net universe object
 * @param step  - current step in the effect
 */  
void effect2(ArtnetWifi* artnet, float step);

/**
 * Calculate the universe for effect 3
 * 
 * @param artnet  - art-net universe object
 * @param step  - current step in the effect
 */  
void effect3(ArtnetWifi* artnet, float step);

/**
 * Calculate the universe for effect 4
 * 
 * @param artnet  - art-net universe object
 * @param step  - current step in the effect
 */  
void effect4(ArtnetWifi* artnet, float step);

/**
 * Calculate the universe for effect 5
 * 
 * @param artnet  - art-net universe object
 * @param step  - current step in the effect
 */  
void effect5(ArtnetWifi* artnet, float step);

/**
 * Calculate the universe for effect 6
 * 
 * @param artnet  - art-net universe object
 * @param step  - current step in the effect
 */  
void effect6(ArtnetWifi* artnet, float step);



#endif