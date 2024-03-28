/**
 * Contains artnet related utilities
 * 
 * Author: Kush Bharakhada and Alex Chapman
 * artnet_utils.cpp
*/

#include <artnet_utils.h>

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ ARTNET UNIVERSE CLASS ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

ArtNetUniverse::ArtNetUniverse(){}

void ArtNetUniverse::begin() {
    for (int i = 0; i < 512; i++) {
        color_universe[i] = 0;
        intensity_universe[i] = 0.0;
        output_universe[i] = 0;
    }
}

void ArtNetUniverse::buildOutputUniverse() {
    for (int i = 0; i < 512; i++) {
        output_universe[i] = intensity_universe[i] * color_universe[i];
    }
}

void ArtNetUniverse::setIntensityUniverse(float *_arr) {
    for (int i = 0; i < 512; i++) {
        intensity_universe[i] = _arr[i];
    }
}

void ArtNetUniverse::setColorUniverse(uint8_t *_arr) {
    for (int i = 0; i < 512; i++) {
        color_universe[i] = _arr[i];
    }
}


void ArtNetUniverse::setIntensity(float _intensity) {
    intensity = _intensity;
}

void ArtNetUniverse::setSpeed(float _speed) {
    speed = _speed;
}


float ArtNetUniverse::getIntensity() {
    return intensity;
}

float ArtNetUniverse::getSpeed() {
    return speed;
}

void ArtNetUniverse::getOutputUniverse(uint8_t* _arr) {
    _arr = output_universe;
}