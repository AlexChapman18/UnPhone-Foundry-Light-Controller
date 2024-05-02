/**
 * Architecture group class, stores RGB values and addresses for each architecture group.
 * 
 * Author: Kush Bharakhada and Alex Chapman (2024)
 * Filename: fixture_utils.h
*/

#ifndef FIXTURE_UTILS_H
#define FIXTURE_UTILS_H

#include <Arduino.h>
#include <stdint.h>

// Arrays of addresses for each pre-defined architecture group
extern uint16_t patch_By_Toilets[];          // 17        
extern uint16_t patch_Main_exit[];           // 11         
extern uint16_t patch_Merch_Downlighters[];  // 3                  
extern uint16_t patch_Pillars[];             // 4       
extern uint16_t patch_Dancefloor_Back[];     // 32               
extern uint16_t patch_Main_Bar_Left[];       // 18             
extern uint16_t patch_Main_Bar_Right[];      // 15              
extern uint16_t patch_Bar_1[];               // 1     
extern uint16_t patch_Bar_2[];               // 1     
extern uint16_t patch_Bar_3[];               // 1     
extern uint16_t patch_Raised_Area_Back[];    // 28                
extern uint16_t patch_Raised_Bar[];          // 19          
extern uint16_t patch_Raised_FOH[];          // 13         
extern uint16_t patch_All_Arcs[];            // 157        
extern uint16_t patch_All_Bars[];            // 3        

extern uint16_t patch_All_Arcs_length;                        
extern uint8_t PATCH_LENGTHS[];            
extern uint16_t *PATCH_ADDRESSES[];                    
          
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ ARCHITECTURE GROUP CLASS ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

class ArchitectureGroup {
    private:
        // Class variables
        const char *name;
        uint8_t num_fixtures;
        uint16_t *addresses; 
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