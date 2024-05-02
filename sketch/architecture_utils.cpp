/**
 * Architecture group class, stores RGB values and addresses for each architecture group.
 * 
 * Author: Kush Bharakhada and Alex Chapman (2024)
 * Filename: architecture_utils.cpp
*/

#include <architecture_utils.h>
#include <artnet_utils.h>

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// ~~~~~~~~~~~~~~~~ ARRAYS OF ADDRESSES FOR EACH PRE-DEFINED ARCHITECTURE GROUP ~~~~~~~~~~~~~~

uint16_t patch_By_Toilets[] = {
  19,22,25,28,31,34,37,40,43,46,49,52,55,58,61,64,67
};
uint16_t patch_Main_exit[] = {
  70,73,76,79,82,85,88,91,94,97,100
};
uint16_t patch_Merch_Downlighters[] = {
  103,106,109
};
uint16_t patch_Pillars[] = {
  112,115,214,217
};
uint16_t patch_Dancefloor_Back[] = {
  118,121,124,127,130,133,136,139,142,145,148,151,154,157,160,163,166,169,
  172,175,178,181,184,187,190,193,196,199, 202,205,208,211
};
uint16_t patch_Main_Bar_Left[] = {
  220,223,226,229,232,235,238,241,244,247,250,253,256,259,262,265,268,271
};
uint16_t patch_Main_Bar_Right[] = {
  274,277,280,283,286,289,292,295,298,301,304,307,310,313,316
};
uint16_t patch_Bar_1[] = {
  319
};
uint16_t patch_Bar_2[] = {
  322
};
uint16_t patch_Bar_3[] = {
  502
};
uint16_t patch_Raised_Area_Back[] = {
  325,325,328,331,334,337,340,343,346,349,352,355,358,361,364,367,370,373,
  376,379,382,385,388,391,394,397,400,403
};
uint16_t patch_Raised_Bar[] = {
  406,409,412,415,418,421,424,427,430,433,436,439,442,445,448,451,454,457,460
};
uint16_t patch_Raised_FOH[] = {
  463,466,469,472,475,478,481,484,487,490,493,496,499
};
uint16_t patch_All_Arcs[] = {
  19,22,25,28,31,34,37,40,43,46,49,52,55,58,61,64,67,70,73,76,79,82,85,88,91,94,97,100,112,115,214,217,118,121,124,127,130,
  133,136,139,142,145,148,151,154,157,160,163,166,169,172,175,178,181,184,187,190,193,196,199,202,205,208,211,220,223,226,
  229,232,235,238,241,244,247,250,253,256,259,262,265,268,271,274,277,280,283,286,289,292,295,298,301,304,307,310,313,316,
  325,325,328,331,334,337,340,343,346,349,352,355,358,361,364,367,370,373,376,379,382,385,388,391,394,397,400,403,406,409,
  412,415,418,421,424,427,430,433,436,439,442,445,448,451,454,457,460,463,466,469,472,475,478,481,484,487,490,493,496,499
};
uint16_t patch_All_Bars[] = {
  319,322,502
};

// General patch information
uint16_t patch_All_Arcs_length = 157;
uint8_t PATCH_LENGTHS[] = {17, 11, 3, 4, 32, 18, 15, 1, 1, 1, 28, 19, 13, 157, 3};
uint16_t *PATCH_ADDRESSES[] = {patch_By_Toilets, patch_Main_exit, patch_Merch_Downlighters, patch_Pillars,
                               patch_Dancefloor_Back, patch_Main_Bar_Left, patch_Main_Bar_Right, patch_Bar_1,
                               patch_Bar_2, patch_Bar_3, patch_Raised_Area_Back, patch_Raised_Bar, patch_Raised_FOH,
                               patch_All_Arcs, patch_All_Bars}; 

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ ARCHITECTURE GROUP CLASS ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

ArchitectureGroup::ArchitectureGroup(){}

ArchitectureGroup::ArchitectureGroup(uint8_t _addresses_index, const char *_name) {
    // Sets all the internal variables
    name = _name;
    num_fixtures = PATCH_LENGTHS[_addresses_index];
    addresses = PATCH_ADDRESSES[_addresses_index];
    ArchitectureGroup::setRGB(0, 0, 0);
}

uint8_t ArtNetUniverse::color_universe[512] = {};

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// ~~~~~~~~~~~~~~~~~~~~~~~  ARCHITECTURE GROUP :: GETTERS AND SETTERS ~~~~~~~~~~~~~~~~~~~~~~~~

void ArchitectureGroup::setRGB(uint8_t _red, uint8_t _green, uint8_t _blue) {
    // Sets R, G and B seperatly
    ArchitectureGroup::setRed(_red);
    ArchitectureGroup::setGreen(_green);
    ArchitectureGroup::setBlue(_blue);
}

void ArchitectureGroup::setRed(uint8_t _red){
    // Sets the internal red value, then sets the color value for each address in the universe
    red = _red;
    for (int i = 0; i < num_fixtures; i++) {
        uint16_t current_address = addresses[i] - 1;
        ArtNetUniverse::color_universe[current_address] = _red;
    }
}

void ArchitectureGroup::setGreen(uint8_t _green){
    // Sets the internal green value, then sets the color value for each address in the universe
    green = _green;
    for (int i = 0; i < num_fixtures; i++) {
        uint16_t current_address = addresses[i];
        ArtNetUniverse::color_universe[current_address] = _green;
    }
}

void ArchitectureGroup::setBlue(uint8_t _blue){
    // Sets the internal blue value, then sets the color value for each address in the universe
    blue = _blue;
    for (int i = 0; i < num_fixtures; i++) {
        uint16_t current_address = addresses[i] + 1;
        ArtNetUniverse::color_universe[current_address] = _blue;
    }
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

const char *ArchitectureGroup::getName() {
    return name;
}
