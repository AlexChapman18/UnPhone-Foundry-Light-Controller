/**
 * Contains all fixture (individual light) classes.
 * 
 * Author: Kush Bharakhada and Alex Chapman
 * fixture_utils.cpp
*/

#include <fixture_utils.h>

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
    red = _red;
    green = _green;
    blue = _blue;
}
void ArchitectureGroup::setRed(uint8_t _red){
    red = _red;
}
void ArchitectureGroup::setGreen(uint8_t _green){
    green = _green;
}
void ArchitectureGroup::setBlue(uint8_t _blue){
    blue = _blue;
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
