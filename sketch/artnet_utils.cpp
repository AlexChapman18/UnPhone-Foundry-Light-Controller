/**
 * Contains artnet related utilities
 *
 * Author: Kush Bharakhada and Alex Chapman
 * artnet_utils.cpp
 */

#include <artnet_utils.h>

ArtnetWifi artnet;

float ArtNetUniverse::intensity_universe[512] =
    {};  // Initialise the empty universe

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ ARTNET UNIVERSE CLASS ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

ArtNetUniverse::ArtNetUniverse() {}

void ArtNetUniverse::setup() {
    for (int i = 0; i < 512; i++) {
        color_universe[i] = 0;
        intensity_universe[i] = 1.0;
    }

    while (WiFi.status() != WL_CONNECTED) {
    }

    intensity = 1;
    speed = 1;

    artnet.begin(TARGET_IP);
    artnet.setUniverse(1);
    artnet.setLength(512);
}

void ArtNetUniverse::begin() {
    xTaskCreate(keepSendingUniverse, "Art-net output", 5000, NULL, 5, NULL);
}

void ArtNetUniverse::setIntensity(float _intensity) {
    intensity = _intensity;
    for (int i = 0; i < 512; i++) {
        intensity_universe[i] = _intensity;
    }
}

void ArtNetUniverse::setSpeed(float _speed) { speed = _speed; }

float ArtNetUniverse::getIntensity() { return intensity; }

float ArtNetUniverse::getSpeed() { return speed; }

void keepSendingUniverse(void *params) {
    while (true) {
        for (int i = 0; i < 512; i++) {
            artnet.setByte(i, ArtNetUniverse::intensity_universe[i] *
                                  ArtNetUniverse::color_universe[i]);
        }
        artnet.write();
        vTaskDelay(60 / portTICK_RATE_MS);  // Wait 60 MS and send again
    }
}