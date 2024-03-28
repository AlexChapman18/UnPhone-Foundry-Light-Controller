/**
 * Contains artnet related utilities
 * 
 * Author: Kush Bharakhada and Alex Chapman
 * artnet_utils.cpp
*/

#include <artnet_utils.h>

ArtnetWifi artnet;

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ ARTNET UNIVERSE CLASS ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

ArtNetUniverse::ArtNetUniverse(){}

void ArtNetUniverse::setup() {
    for (int i = 0; i < 512; i++) {
        color_universe[i] = 0;
        intensity_universe[i] = 0.0;
        output_universe[i] = 0;
    }

    while (WiFi.status() != WL_CONNECTED){}

    artnet.begin(TARGET_IP);
    artnet.setUniverse(1);
    artnet.setLength(512);
}

void ArtNetUniverse::begin() {
    xTaskCreate(
        keepSendingUniverse,
        "Art-net output",
        5000,
        NULL,
        5,
        NULL
    );
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


uint8_t ArtNetUniverse::output_universe[512] = {}; // Initialise the empty universe
void keepSendingUniverse(void *params) {
    while (true) {
        for (int i = 0; i < 512; i++) {
            artnet.setByte(i, ArtNetUniverse::output_universe[i]);
        }
        artnet.write();
        vTaskDelay(60/portTICK_RATE_MS); // Wait 30 MS and send again
    }
}