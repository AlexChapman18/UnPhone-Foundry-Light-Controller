/**
 * Contains artnet related utilities
 *
 * Author: Kush Bharakhada and Alex Chapman
 * artnet_utils.cpp
 */

#include <artnet_utils.h>

ArtnetWifi artnet;

float ArtNetUniverse::intensity_universe[512] = {};  // Initialise the empty universe
uint8_t ArtNetUniverse::current_effect = 0;
uint8_t ArtNetUniverse::current_speed = 0;


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

    current_effect = 0;
    current_speed = 1;

    artnet.begin(TARGET_IP);
    artnet.setUniverse(1);
    artnet.setLength(512);
}

void ArtNetUniverse::begin() {
    xTaskCreate(keepSendingUniverse, "Art-net output", 7000, NULL, configMAX_PRIORITIES - 1, NULL);
}

void ArtNetUniverse::setIntensity(float _intensity) {
    intensity = _intensity;
    if (current_effect == 0){
        for (int i = 0; i < 512; i++) {
            intensity_universe[i] = _intensity;
        }
    }
}

void ArtNetUniverse::setSpeed(float _speed) { current_speed = _speed; }

float ArtNetUniverse::getSpeed() { return current_speed; }

float ArtNetUniverse::getIntensity() { return intensity; }

void keepSendingUniverse(void *params) {
    while (true) {
        float step = 0;

        // Effect 1
        while (ArtNetUniverse::current_effect == 0){
            for (int i = 0; i < 512; i++) {
                artnet.setByte(i, ArtNetUniverse::intensity_universe[i] *
                                    ArtNetUniverse::color_universe[i]);
            }
            artnet.write();
            vTaskDelay(60 / portTICK_RATE_MS);
        }

        // Effect 2
        while (ArtNetUniverse::current_effect == 1){

            float value = abs(sin(step/(100 + (200 * (1 - ArtNetUniverse::current_speed)))));
            for (int i = 0; i < 512; i++) {
                artnet.setByte(i, value * ArtNetUniverse::color_universe[i]);
            }
            step++;
            artnet.write();
            vTaskDelay(60 / portTICK_RATE_MS); // Wait 60 MS and send again
        }

        
        // Effect 3
        while (ArtNetUniverse::current_effect == 2){

            float speed = 1 / (25 + 200 * (1 - (float)ArtNetUniverse::current_speed));
            for (int i = 0; i < patch_All_Arcs_length; i++) {

                float offset = ((float)i / (float)patch_All_Arcs_length) * 3.14;
                float index = step * speed + offset;
                float intensity = abs(sin(index));

                uint16_t red_address = patch_All_Arcs[i] - 1;
                uint16_t green_address = patch_All_Arcs[i];
                uint16_t blue_address = patch_All_Arcs[i] + 1;

                artnet.setByte(red_address, intensity * ArtNetUniverse::color_universe[red_address]);
                artnet.setByte(green_address, intensity * ArtNetUniverse::color_universe[green_address]);
                artnet.setByte(blue_address, intensity * ArtNetUniverse::color_universe[blue_address]);
            }
            step++;
            artnet.write();
            vTaskDelay(60 / portTICK_RATE_MS); // Wait 60 MS and send again
        }
        vTaskDelay(100 / portTICK_RATE_MS);
    }
}