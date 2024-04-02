/**
 * Contains art-net related utilities
 * 
 * Author: Kush Bharakhada and Alex Chapman
 * artnet_utils.h
*/

#ifndef ARTNET_UTILS_H
#define ARTNET_UTILS_H

#include <Arduino.h>
#include <stdint.h>
#include <ArtnetWifi.h>
#include <private.h>
#include <fixture_utils.h>

void keepSendingUniverse(void *params);
void keepRunningEffects(void *params);

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ ARTNET UNIVERSE CLASS ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

class ArtNetUniverse {
    public:
        ArtNetUniverse();

        void setup();
        void begin();
        
        void setIntensity(float _intensity);
        void setSpeed(float _speed);

        float getIntensity();
        float getSpeed();

        static uint8_t color_universe[512];
        static uint8_t current_effect;
        static float current_speed;
        static float current_intensity;
};


#endif