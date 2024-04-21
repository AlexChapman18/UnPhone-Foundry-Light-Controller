/**
 * Contains Artnet related utilities.
 *
 * Author: Kush Bharakhada and Alex Chapman (2024)
 * Filename: fixture_utils.cpp
*/

#include <artnet_utils.h>

void keepSendingUniverse(void *params) {
    // Initialised the values needed to calculate effects
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
        // Runs the current effect function
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

        // Sends the calcualted universe as a network packet
        artnet.write();
        // Wait 35 MS between sending packets
        vTaskDelay(35 / portTICK_RATE_MS);
    }
}

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ ARTNET UNIVERSE CLASS ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

ArtnetWifi artnet;

uint8_t ArtNetUniverse::current_effect = 0;
float ArtNetUniverse::current_speed = 0;
float ArtNetUniverse::current_intensity = 0;

ArtNetUniverse::ArtNetUniverse() {}

void ArtNetUniverse::setup() {
    // Sets class variables
    current_effect = 0;
    current_speed = 1;
    current_intensity = 1;

    // Sets the target IP, target universe number and universe length
    artnet.begin(TARGET_IP);
    artnet.setUniverse(5);
    artnet.setLength(512);          
}

void ArtNetUniverse::begin() {
    // Creates a new thread running keepSendingUniverse function
    xTaskCreate(keepSendingUniverse, "Art-net output", 7000, NULL, configMAX_PRIORITIES - 1, NULL);
}

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ ARTNET :: GETTERS AND SETTERS ~~~~~~~~~~~~~~~~~~~~~~~~~~

void ArtNetUniverse::setIntensity(float _intensity) { 
    current_intensity = _intensity; 
}

void ArtNetUniverse::setSpeed(float _speed) { 
    current_speed = _speed;
}

float ArtNetUniverse::getSpeed() { 
    return current_speed; 
}

float ArtNetUniverse::getIntensity() { 
    return current_intensity; 
}

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ EFFECTS ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

void effect1(ArtnetWifi *artnet) {
    // Iterates through each value in the universe, multiplies its respective color
    // and intensity values and writes it to artnet
    for (int i = 0; i < 512; i++) {
        artnet->setByte(i, ArtNetUniverse::current_intensity * ArtNetUniverse::color_universe[i]);
    }
}

void effect2(ArtnetWifi *artnet, float step) {
    // calculates the new intensity value based on the current step
    float intensity = abs(sin(step/(10 + (30 * (1 - ArtNetUniverse::current_speed))))) * ArtNetUniverse::current_intensity;
    
    for (int i = 0; i < 512; i++) {
        // Writes the multiplied value to art-net
        artnet->setByte(i, intensity * ArtNetUniverse::color_universe[i]);
    }
}

void effect3(ArtnetWifi *artnet, float step) {
    // Calcualtes the speed the effect should run based on the speed fader values
    float speed = 1 / (20 + 30 * (1 - (float)ArtNetUniverse::current_speed));
    
    // Itterates through each address in the arcs
    for (int i = 0; i < patch_All_Arcs_length; i++) {
        
        // Calculates the new intensity based on intesnity value, step and current arc index
        float offset = i%2 * 3.14 / 2;
        float index = step * speed + offset;
        float intensity = abs(sin(index)) * ArtNetUniverse::current_intensity;

        // Gets the address of R,G,B for the current arc
        uint16_t red_address = patch_All_Arcs[i] - 1;
        uint16_t green_address = patch_All_Arcs[i];
        uint16_t blue_address = patch_All_Arcs[i] + 1;

        // Sets the value of RGB seperatly
        artnet->setByte(red_address, intensity * ArtNetUniverse::color_universe[red_address]);
        artnet->setByte(green_address, intensity * ArtNetUniverse::color_universe[green_address]);
        artnet->setByte(blue_address, intensity * ArtNetUniverse::color_universe[blue_address]);
    }
}

void effect4(ArtnetWifi *artnet, float step) {
    // Calcualtes the speed the effect should run based on the speed fader values
    float speed = 1 / (20 + 30 * (1 - (float)ArtNetUniverse::current_speed));

    for (int i = 0; i < patch_All_Arcs_length; i++) {
        // Calculates the new intensity based on intesnity value, step and current arc index
        float offset = ((float)i / (float)patch_All_Arcs_length) * 3.14;
        float index = step * speed + offset;
        float intensity = abs(sin(index)) * ArtNetUniverse::current_intensity;

        // Gets the address of R,G,B for the current arc
        uint16_t red_address = patch_All_Arcs[i] - 1;
        uint16_t green_address = patch_All_Arcs[i];
        uint16_t blue_address = patch_All_Arcs[i] + 1;

        // Sets the value of RGB seperatly
        artnet->setByte(red_address, intensity * ArtNetUniverse::color_universe[red_address]);
        artnet->setByte(green_address, intensity * ArtNetUniverse::color_universe[green_address]);
        artnet->setByte(blue_address, intensity * ArtNetUniverse::color_universe[blue_address]);
    }
}

void effect5(ArtnetWifi *artnet, float step) {
    // Calcualtes the speed the effect should run based on the speed fader values
    float speed = 1 / (20 + 30 * (1 - (float)ArtNetUniverse::current_speed));
    for (int i = 0; i < patch_All_Arcs_length; i++) {

        // Calculates the new intensity based on intesnity value, step and current arc index
        float offset = ((float)i / (float)patch_All_Arcs_length) * 3.14;
        float index = step * speed + offset;
        float intensity = round(abs(sin(index))) * ArtNetUniverse::current_intensity;

        // Gets the address of R,G,B for the current arc
        uint16_t red_address = patch_All_Arcs[i] - 1;
        uint16_t green_address = patch_All_Arcs[i];
        uint16_t blue_address = patch_All_Arcs[i] + 1;

        // Sets the value of RGB seperatly
        artnet->setByte(red_address, intensity * ArtNetUniverse::color_universe[red_address]);
        artnet->setByte(green_address, intensity * ArtNetUniverse::color_universe[green_address]);
        artnet->setByte(blue_address, intensity * ArtNetUniverse::color_universe[blue_address]);
    }
}

void effect6(ArtnetWifi *artnet, float step) {
    effect1(artnet);
    // Calculates the speed the effect should run based on the speed fader values
    float speed = 1 / (20 + 30 * (1 - (float)ArtNetUniverse::current_speed));

    // Iterates through each bar
    for (int i = 0; i < 3; i++) {
        // Calculates the new intensity based on intesnity value, step and current arc index
        float offset = ((float)i / (float)3) * 3.14;
        float index = step * speed + offset;
        float intensity = abs(sin(index)) * ArtNetUniverse::current_intensity;

        // Gets the address of R,G,B for the current arc
        uint16_t red_address = patch_All_Bars[i] - 1;
        uint16_t green_address = patch_All_Bars[i];
        uint16_t blue_address = patch_All_Bars[i] + 1;

        // Sets the value of RGB seperatly
        artnet->setByte(red_address, intensity * ArtNetUniverse::color_universe[red_address]);
        artnet->setByte(green_address, intensity * ArtNetUniverse::color_universe[green_address]);
        artnet->setByte(blue_address, intensity * ArtNetUniverse::color_universe[blue_address]);
    }
}