/**
 * Contains all fixture (individual light) classes.
 * 
 * Author: Kush Bharakhada and Alex Chapman
 * fixture_utils.h
*/

#ifndef FIXTURE_UTILS_H
#define FIXTURE_UTILS_H

#include <Arduino.h>
#include <stdint.h>

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ FIXTURE CLASS ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

class Fixture {
    public:
        Fixture();
        Fixture(uint8_t _address);
        
        uint8_t red;
        uint8_t green;
        uint8_t blue;
        uint8_t address;

        void setRed(uint8_t _red);
        void setGreen(uint8_t _green);
        void setBlue(uint8_t _blue);
        void setRGB(uint8_t _red, uint8_t _green, uint8_t _blue);

        uint8_t getRed();
        uint8_t getGreen();
        uint8_t getBlue();
        uint8_t getAddress();

        void getRGB(uint8_t *_arr);
};

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ FIXTURE GROUP CLASS ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

// FixtureGroup(i, "Toilets")
// Get the name
// Set individual RGB
// Get individual RGB

class FixtureGroup {
    public:
        FixtureGroup();
        FixtureGroup(Fixture* _fixtures, uint8_t _num_fixtures);
        
        uint8_t num_fixtures;
        Fixture* fixtures;

        void setGlobalRGB(uint8_t _red, uint8_t _green, uint8_t _blue);
        void setGlobalRed(uint8_t _red);
        void setGlobalGreen(uint8_t _green);
        void setGlobalBlue(uint8_t _blue);

        Fixture* getFixtures();
        uint8_t getNumFixtures();
        uint8_t getGlobalRed();
        uint8_t getGlobalGreen();
        uint8_t getGlobalBlue();

        
        void print();
};

#endif