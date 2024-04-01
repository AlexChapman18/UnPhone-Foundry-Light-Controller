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

// 17, 11, 3, 4, 32, 18, 15, 1, 1, 1, 28, 19, 13, 157, 3
extern uint16_t patch_By_Toilets[];
extern uint16_t patch_Main_exit[];
extern uint16_t patch_Merch_Downlighters[];
extern uint16_t patch_Pillars[];
extern uint16_t patch_Dancefloor_Back[];
extern uint16_t patch_Main_Bar_Left[];
extern uint16_t patch_Main_Bar_Right[];
extern uint16_t patch_Bar_1[];
extern uint16_t patch_Bar_2[];
extern uint16_t patch_Bar_3[];
extern uint16_t patch_Raised_Area_Back[];
extern uint16_t patch_Raised_Bar[];
extern uint16_t patch_Raised_FOH[];
extern uint16_t patch_All_Arcs[];
extern uint16_t patch_All_Bars[];
extern uint16_t patch_All_Arcs_length;
extern uint8_t PATCH_LENGTHS[];
extern uint16_t *PATCH_ADDRESSES[];

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ ARCHITECTURE GROUP CLASS ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

class ArchitectureGroup {
    private:
        const char* name;

        uint8_t num_fixtures;
        uint16_t *addresses;

        uint8_t red;
        uint8_t green;
        uint8_t blue;

    public:
        ArchitectureGroup();
        ArchitectureGroup(uint8_t _addresses_index, const char* _name);

        void setRGB(uint8_t _red, uint8_t _green, uint8_t _blue);
        void setRed(uint8_t _red);
        void setGreen(uint8_t _green);
        void setBlue(uint8_t _blue);

        uint16_t* getAddresses();
        uint8_t getNumFixtures();
        uint8_t getRed();
        uint8_t getGreen();
        uint8_t getBlue();
        const char* getName();
};

#endif