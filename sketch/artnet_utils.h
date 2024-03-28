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

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ ARTNET UNIVERSE CLASS ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

class ArtNetUniverse {
    public:
        ArtNetUniverse();

        void begin();
        void buildOutputUniverse();
        
        void setIntensity(float _intensity);
        void setSpeed(float _speed);
        void setColorUniverse(uint8_t *_arr);
        void setIntensityUniverse(float *_arr);

        float getIntensity();
        float getSpeed();
        void getOutputUniverse(uint8_t* _arr);

    private:
        uint8_t color_universe[512];
        float intensity_universe[512];
        uint8_t output_universe[512];
        
        float intensity;
        float speed;
};


#endif