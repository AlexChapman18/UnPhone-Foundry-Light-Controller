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
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ ARCHITECTURE GROUP CLASS ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

class ArchitectureGroup {
    private:
        const char* name;

        uint8_t num_fixtures;
        uint8_t* addresses;

        uint8_t red;
        uint8_t green;
        uint8_t blue;

    public:
        ArchitectureGroup();
        ArchitectureGroup(uint8_t* _addresses, uint8_t _num_fixtures, const char* _name);

        void setRGB(uint8_t _red, uint8_t _green, uint8_t _blue);
        void setRed(uint8_t _red);
        void setGreen(uint8_t _green);
        void setBlue(uint8_t _blue);

        uint8_t* getAddresses();
        uint8_t getNumFixtures();
        uint8_t getRed();
        uint8_t getGreen();
        uint8_t getBlue();
        const char* getName();
};

#endif