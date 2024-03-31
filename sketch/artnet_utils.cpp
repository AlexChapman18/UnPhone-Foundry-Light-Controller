/**
 * Contains artnet related utilities
 *
 * Author: Kush Bharakhada and Alex Chapman
 * artnet_utils.cpp
 */

#include <artnet_utils.h>

ArtnetWifi artnet;

uint8_t ArtNetUniverse::current_effect = 0;
uint8_t ArtNetUniverse::current_speed = 0;
float ArtNetUniverse::current_intensity = 0;


// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ ARTNET UNIVERSE CLASS ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

ArtNetUniverse::ArtNetUniverse() {}

void ArtNetUniverse::setup() {

    while (WiFi.status() != WL_CONNECTED) {
    }

    current_effect = 0;
    current_speed = 1;
    current_intensity = 1;

    artnet.begin(TARGET_IP);
    artnet.setUniverse(1);
    artnet.setLength(512);
}

void ArtNetUniverse::begin() {
    xTaskCreate(keepSendingUniverse, "Art-net output", 7000, NULL, configMAX_PRIORITIES - 1, NULL);
}

void ArtNetUniverse::setIntensity(float _intensity) {
    current_intensity = _intensity;
}

void ArtNetUniverse::setSpeed(float _speed) { current_speed = _speed; }

float ArtNetUniverse::getSpeed() { return current_speed; }

float ArtNetUniverse::getIntensity() { return current_intensity; }

void keepSendingUniverse(void *params) {
    while (true) {
        float step = 0;

        // Effect 1
        while (ArtNetUniverse::current_effect == 0){
            for (int i = 0; i < 512; i++) {
                artnet.setByte(i, ArtNetUniverse::current_intensity *
                                    ArtNetUniverse::color_universe[i]);
            }
            artnet.write();
            vTaskDelay(60 / portTICK_RATE_MS);
        }

        // Effect 2
        while (ArtNetUniverse::current_effect == 1){

            float value = abs(sin(step/(100 + (200 * (1 - ArtNetUniverse::current_speed))))) * ArtNetUniverse::current_intensity;
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

                float offset = i%2 * 3.14 / 2;
                float index = step * speed + offset;
                float intensity = abs(sin(index)) * ArtNetUniverse::current_intensity;

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

        
        // Effect 4
        while (ArtNetUniverse::current_effect == 3){

            float speed = 1 / (25 + 200 * (1 - (float)ArtNetUniverse::current_speed));
            for (int i = 0; i < patch_All_Arcs_length; i++) {

                float offset = ((float)i / (float)patch_All_Arcs_length) * 3.14;
                float index = step * speed + offset;
                float intensity = abs(sin(index)) * ArtNetUniverse::current_intensity;

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