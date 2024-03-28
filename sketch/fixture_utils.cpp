/**
 * Contains all fixture (individual light) classes.
 * 
 * Author: Kush Bharakhada and Alex Chapman
 * fixture_utils.cpp
*/

#include <fixture_utils.h>


#include <artnet_utils.h>
uint8_t ArtNetUniverse::color_universe[512] = {};

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ ARCHITECTURE GROUP CLASS ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

// Constructors
ArchitectureGroup::ArchitectureGroup(){}

ArchitectureGroup::ArchitectureGroup(uint8_t *_addresses, uint8_t _num_fixtures, const char* _name) {
    name = _name;
    num_fixtures = _num_fixtures;
    addresses = _addresses;
    ArchitectureGroup::setRGB(0, 0, 0);
}

// Setters
void ArchitectureGroup::setRGB(uint8_t _red, uint8_t _green, uint8_t _blue) {
    ArchitectureGroup::setRed(_red);
    ArchitectureGroup::setGreen(_green);
    ArchitectureGroup::setBlue(_blue);
}
void ArchitectureGroup::setRed(uint8_t _red){
    red = _red;

    for (int i = 0; i < num_fixtures; i++) {
        uint8_t current_address = addresses[i] - 1;
        ArtNetUniverse::color_universe[current_address] = _red;
    }
}
void ArchitectureGroup::setGreen(uint8_t _green){
    green = _green;
    for (int i = 0; i < num_fixtures; i++) {
        uint8_t current_address = addresses[i];
        ArtNetUniverse::color_universe[current_address] = _green;
    }
}
void ArchitectureGroup::setBlue(uint8_t _blue){
    blue = _blue;
    for (int i = 0; i < num_fixtures; i++) {
        uint8_t current_address = addresses[i] + 1;
        ArtNetUniverse::color_universe[current_address] = _blue;
    }
}

// Getters
uint8_t* ArchitectureGroup::getAddresses() {
    return addresses;
}
uint8_t ArchitectureGroup::getNumFixtures() {
    return num_fixtures;
}
uint8_t ArchitectureGroup::getRed() {
    return red;
}
uint8_t ArchitectureGroup::getGreen() {
    return green;
}
uint8_t ArchitectureGroup::getBlue() {
    return blue;
}
const char* ArchitectureGroup::getName() {
    return name;
}
