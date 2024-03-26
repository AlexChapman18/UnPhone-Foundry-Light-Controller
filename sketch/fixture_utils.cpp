/**
 * Contains all fixture (individual light) classes.
 * 
 * Author: Kush Bharakhada and Alex Chapman
 * fixture_utils.cpp
*/

#include <fixture_utils.h>

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ FIXTURE CLASS ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~


Fixture::Fixture() {
    red = 0;
    green = 0;
    blue = 0;
}
Fixture::Fixture(uint8_t _address) {
    address = _address;
    red = 0;
    green = 0;
    blue = 0;
}

void Fixture::setRed(uint8_t _red){
    red = _red;
}
void Fixture::setGreen(uint8_t _green){
    green = _green;
}
void Fixture::setBlue(uint8_t _blue){
    blue = _blue;
}
void Fixture::setRGB(uint8_t _red, uint8_t _green, uint8_t _blue) {
    red = _red;
    green = _green;
    blue = _blue;
}

uint8_t Fixture::getRed() {
    return red;
}
uint8_t Fixture::getGreen() {
    return green;
}
uint8_t Fixture::getBlue() {
    return blue;
}
uint8_t Fixture::getAddress() {
    return address;
}
uint8_t Fixture::getRGB(uint8_t *_arr) {
    _arr[0] = red;
    _arr[1] = green;
    _arr[2] = blue;
}


// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ FIXTURE GROUP CLASS ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

// Constructor
FixtureGroup::FixtureGroup(Fixture *_fixtures, uint8_t _num_fixtures) {
    num_fixtures = _num_fixtures;
    fixtures = _fixtures;
}

FixtureGroup::FixtureGroup(){}

// Setters
void FixtureGroup::setGlobalRGB(uint8_t _red, uint8_t _green, uint8_t _blue) {
    for (int i = 0; i < num_fixtures; ++i) {
        fixtures[i].setRGB(_red, _green, _blue);
    }
}

// Getters
Fixture* FixtureGroup::getFixtures() {
    return fixtures;
}
uint8_t FixtureGroup::getNumFixtures() {
    return num_fixtures;
}

// Other
void FixtureGroup::print() {
  Serial.print("addresses: ");
  for (int i = 0; i < num_fixtures; i++) {
    Serial.print(fixtures[i].getAddress());
    Serial.print(" ");
  }
  Serial.print("red: ");
  for (int i = 0; i < num_fixtures; i++) {
    Serial.print(fixtures[i].getRed());
    Serial.print(" ");
  }
  Serial.println();
}
