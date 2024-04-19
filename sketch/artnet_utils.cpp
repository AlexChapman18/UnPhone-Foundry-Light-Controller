/**
 * Contains artnet related utilities
 *
 * Author: Kush Bharakhada and Alex Chapman
 * artnet_utils.cpp
 */

#include <artnet_utils.h>

ArtnetWifi artnet;

uint8_t ArtNetUniverse::current_effect = 0;
float ArtNetUniverse::current_speed = 0;
float ArtNetUniverse::current_intensity = 0;


// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ ARTNET UNIVERSE CLASS ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

ArtNetUniverse::ArtNetUniverse() {}

void ArtNetUniverse::setup() {

    current_effect = 0;
    current_speed = 1;
    current_intensity = 1;

    artnet.begin(TARGET_IP);
    artnet.setUniverse(5);
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

// Effect 1, Solid
void effect1(ArtnetWifi* artnet) {
    for (int i = 0; i < 512; i++) {
    artnet->setByte(i, ArtNetUniverse::current_intensity *
                        ArtNetUniverse::color_universe[i]);
    }
}

// Effect 2, Pulse
void effect2(ArtnetWifi* artnet, float step) {
    float value = abs(sin(step/(10 + (30 * (1 - ArtNetUniverse::current_speed))))) * ArtNetUniverse::current_intensity;
    for (int i = 0; i < 512; i++) {
        artnet->setByte(i, value * ArtNetUniverse::color_universe[i]);
    }
}

// Effect 3, Odd even
void effect3(ArtnetWifi* artnet, float step) {
    float speed = 1 / (20 + 30 * (1 - (float)ArtNetUniverse::current_speed));
    for (int i = 0; i < patch_All_Arcs_length; i++) {

        float offset = i%2 * 3.14 / 2;
        float index = step * speed + offset;
        float intensity = abs(sin(index)) * ArtNetUniverse::current_intensity;

        uint16_t red_address = patch_All_Arcs[i] - 1;
        uint16_t green_address = patch_All_Arcs[i];
        uint16_t blue_address = patch_All_Arcs[i] + 1;

        artnet->setByte(red_address, intensity * ArtNetUniverse::color_universe[red_address]);
        artnet->setByte(green_address, intensity * ArtNetUniverse::color_universe[green_address]);
        artnet->setByte(blue_address, intensity * ArtNetUniverse::color_universe[blue_address]);
    }
}

// Effect 4, Fade Swipe
void effect4(ArtnetWifi* artnet, float step) {
    float speed = 1 / (20 + 30 * (1 - (float)ArtNetUniverse::current_speed));
    for (int i = 0; i < patch_All_Arcs_length; i++) {

        float offset = ((float)i / (float)patch_All_Arcs_length) * 3.14;
        float index = step * speed + offset;
        float intensity = abs(sin(index)) * ArtNetUniverse::current_intensity;

        uint16_t red_address = patch_All_Arcs[i] - 1;
        uint16_t green_address = patch_All_Arcs[i];
        uint16_t blue_address = patch_All_Arcs[i] + 1;

        artnet->setByte(red_address, intensity * ArtNetUniverse::color_universe[red_address]);
        artnet->setByte(green_address, intensity * ArtNetUniverse::color_universe[green_address]);
        artnet->setByte(blue_address, intensity * ArtNetUniverse::color_universe[blue_address]);
    }
}

// Effect 5, binary swipe
void effect5(ArtnetWifi* artnet, float step) {
    float speed = 1 / (20 + 30 * (1 - (float)ArtNetUniverse::current_speed));
    for (int i = 0; i < patch_All_Arcs_length; i++) {

        float offset = ((float)i / (float)patch_All_Arcs_length) * 3.14;
        float index = step * speed + offset;
        float intensity = round(abs(sin(index))) * ArtNetUniverse::current_intensity;

        uint16_t red_address = patch_All_Arcs[i] - 1;
        uint16_t green_address = patch_All_Arcs[i];
        uint16_t blue_address = patch_All_Arcs[i] + 1;

        artnet->setByte(red_address, intensity * ArtNetUniverse::color_universe[red_address]);
        artnet->setByte(green_address, intensity * ArtNetUniverse::color_universe[green_address]);
        artnet->setByte(blue_address, intensity * ArtNetUniverse::color_universe[blue_address]);
    }
}

// Effect 6, Bars Fade
void effect6(ArtnetWifi* artnet, float step) {
    effect1(artnet);
    float speed = 1 / (20 + 30 * (1 - (float)ArtNetUniverse::current_speed));
    for (int i = 0; i < 3; i++) {

        float offset = ((float)i / (float)3) * 3.14;
        float index = step * speed + offset;
        float intensity = abs(sin(index)) * ArtNetUniverse::current_intensity;

        uint16_t red_address = patch_All_Bars[i] - 1;
        uint16_t green_address = patch_All_Bars[i];
        uint16_t blue_address = patch_All_Bars[i] + 1;

        artnet->setByte(red_address, intensity * ArtNetUniverse::color_universe[red_address]);
        artnet->setByte(green_address, intensity * ArtNetUniverse::color_universe[green_address]);
        artnet->setByte(blue_address, intensity * ArtNetUniverse::color_universe[blue_address]);
    }
}


// Thread for sending the universe
void keepSendingUniverse(void *params) {
    uint8_t last_effect = 0;
    float step = 0;
    float value;
    float speed;

    while (true) {
        
        // Everytime you change effect, reset the step counter
        if (last_effect != ArtNetUniverse::current_effect) {
            step = 0;
            last_effect = ArtNetUniverse::current_effect;
        } else {
            step++;
        }

        switch (ArtNetUniverse::current_effect) {
            case 0:
                effect1(&artnet);
                break;
            case 1:
                effect2(&artnet, step);
                break;
            case 2:
                effect3(&artnet, step);
                break; 
            case 3:
                effect4(&artnet, step);
                break;
            case 4:
                effect5(&artnet, step);
                break;
            case 5:
                effect6(&artnet, step);
                break;
        }
        artnet.write();
        vTaskDelay(35 / portTICK_RATE_MS); // Wait 35 MS between sending packets
    }
}