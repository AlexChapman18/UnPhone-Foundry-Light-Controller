/**
 * Architecture group class, stores RGB values and addresses for each architecture group.
 * 
 * Author: Kush Bharakhada and Alex Chapman (2024)
 * Filename: architecture_utils.h
*/

#ifndef ARCHITECTURE_UTILS_H
#define ARCHITECTURE_UTILS_H

#include <Arduino.h>
#include <stdint.h>

// Arrays of addresses for each pre-defined architecture group
extern const uint16_t PATCH_BY_TOILETS[];          // 17        
extern const uint16_t PATCH_MAIN_EXIT[];           // 11         
extern const uint16_t PATCH_MERCH_DOWNLIGHTERS[];  // 3                  
extern const uint16_t PATCH_PILLARS[];             // 4       
extern const uint16_t PATCH_DANCEFLOOR_BACK[];     // 32               
extern const uint16_t PATCH_MAIN_BAR_LEFT[];       // 18             
extern const uint16_t PATCH_MAIN_BAR_RIGHT[];      // 15              
extern const uint16_t PATCH_BAR_1[];               // 1     
extern const uint16_t PATCH_BAR_2[];               // 1     
extern const uint16_t PATCH_BAR_3[];               // 1     
extern const uint16_t PATCH_RAISED_AREA_BACK[];    // 28                
extern const uint16_t PATCH_RAISED_BAR[];          // 19          
extern const uint16_t PATCH_RAISED_FOH[];          // 13         
extern const uint16_t PATCH_ALL_ARCS[];            // 157        
extern const uint16_t PATCH_ALL_BARS[];            // 3        

extern const uint16_t PATCH_ALL_ARCS_LENGTH;                        
extern const uint8_t PATCH_LENGTHS[];            
extern const uint16_t *PATCH_ADDRESSES[];                    
          
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ ARCHITECTURE GROUP CLASS ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

class ArchitectureGroup {
    private:
        // Class variables
        const char *name;
        uint8_t num_fixtures;
        const uint16_t *addresses; 
        uint8_t red;
        uint8_t green;
        uint8_t blue;

    public:
        /**
         * Architecture group constructor.
         */
        ArchitectureGroup();

        /**
         * Architecture group constructor.
         * 
         * @param _addresses_index - Index of the array of addresses.
         * @param _name            - Bame of acrchitecture group.
         */
        ArchitectureGroup(uint8_t _addresses_index, const char *_name);

        /**
         * Setter for Red, Green and Blue
         * 
         * @param _red   - New red value.
         * @param _green - New green value.
         * @param _blue  - New blue value.
         */
        void setRGB(uint8_t _red, uint8_t _green, uint8_t _blue);

        /**
         * Setter for Red.
         * 
         * @param _red - New red value.
         */
        void setRed(uint8_t _red);

        /**
         * Setter for Green.
         * 
         * @param _green - New green value.
         */
        void setGreen(uint8_t _green);

        /**
         * Setter for Blue.
         * 
         * @param _blue - New blue value.
         */
        void setBlue(uint8_t _blue);

        /**
         * Getter for Red.
         * 
         * @return - Gets the current red value.
         */
        uint8_t getRed();

        /**
         * Getter for Green
         * 
         * @return - Gets the current green value.
         */
        uint8_t getGreen();

        /**
         * Getter for Blue.
         * 
         * @return - Gets the current blue value .
         */
        uint8_t getBlue();

        /**
         * Getter for Name.
         * 
         * @return - Gets the current architecture group name. 
         */
        const char *getName();
};

#endif