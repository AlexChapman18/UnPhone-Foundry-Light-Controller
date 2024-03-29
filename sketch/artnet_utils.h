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

void keepSendingUniverse(void *params);

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
        static float intensity_universe[512];

    private:
        
        float intensity;
        float speed;
};


#endif