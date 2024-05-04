/**
 * A reduced version of unPhone.cpp.
 * 
 * Author: Kush Bharakhada and Alex Chapman (2024)
 * Filename: nuPhone.cpp
*/

#include <NuPhone.h>

#define POWER_SWITCH 18
static const uint8_t BUTTON1 = 45;   // Left button
static const uint8_t BUTTON2 = 0;    // Middle button
static const uint8_t BUTTON3 = 21;   // Right button

// LCD Pins
static const uint8_t BACKLIGHT = 2 | 0x40;
static const uint8_t LCD_CS = 48;
static const uint8_t LCD_DC = 47;
static const uint8_t TOUCH_CS = 38;
static const uint8_t LCD_RESET = 46;

void continuousPowerCheck(void *param) ;

#define TCA9555_ADDRESS 0x26

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ NUPHONE CLASS ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

NuPhone::NuPhone() { me = this; }

NuPhone *NuPhone::me = NULL;

void NuPhone::begin() {

    checkPowerSwitch();
    // Setup buttons
    pinMode(BUTTON1, INPUT_PULLUP);
    pinMode(BUTTON2, INPUT_PULLUP);
    pinMode(BUTTON3, INPUT_PULLUP);

    // Setup backlight
    NuPhone::initializeTCAChip();

    // Setup touchscreen
    tsp = new XPT2046_Touchscreen(TOUCH_CS);

    // Start power switch checking
    xTaskCreate(continuousPowerCheck, "power switch task", 4096, NULL, 1, NULL);
    tsp->begin();
}

void NuPhone::initializeTCAChip(){
    Wire.begin();
    Wire.beginTransmission(TCA9555_ADDRESS); // Transmit to the TCA chip
    Wire.write(0x06);                        // Talk to Configuration Registers 1
    Wire.write(0b11111011);                  // Set the backlight as an output pin
    Wire.write(0xFF);                        // Set the other pins to Input
    Wire.endTransmission();
}

void NuPhone::checkPowerSwitch() {
  // Check if the power switch in the off position
    if (!digitalRead(POWER_SWITCH)) {
        // If so, enable "Wake on power switch" and go to sleep
        NuPhone::setBacklight(false);
        esp_sleep_enable_ext0_wakeup((gpio_num_t) POWER_SWITCH, 1);
        esp_deep_sleep_start();
    }
}

void NuPhone::setBacklight(bool shouldBacklight) {
    Wire.beginTransmission(TCA9555_ADDRESS);     // Transmit to the TCA Chip
    Wire.write(0x02);                            // Specify the Output Port Register 1
    if (shouldBacklight) Wire.write(0b00000100); // Turn on the backlight output pin
    else Wire.write(0b00000000);                 // Turn off the backlight output pin
    Wire.endTransmission();
}

void continuousPowerCheck(void *param) {
  // Check power switch every 10th of sec
  while (true) { NuPhone::me->checkPowerSwitch(); delay(100); }
}

bool NuPhone::isButton1() { 
    // Checks if the button is pressed, if it is pressed set a value saying it is being held
    // If the button status is checked again, it will read as false, to avoid double reading
    // of the pressed value.
    bool isPressed = digitalRead(BUTTON1) == LOW;
    if (isPressed && isButton1Held) {
        return false;
    }
    isButton1Held = isPressed;
    return isPressed; 
}

bool NuPhone::isButton2() { 
    // Checks if the button is pressed, if it is pressed set a value saying it is being held
    // If the button status is checked again, it will read as false, to avoid double reading
    // of the pressed value.
    bool isPressed = digitalRead(BUTTON2) == LOW;
    if (isPressed && isButton2Held) {
        return false;
    }
    isButton2Held = isPressed;
    return isPressed; 
}

bool NuPhone::isButton3() { 
    // Checks if the button is pressed, if it is pressed set a value saying it is being held
    // If the button status is checked again, it will read as false, to avoid double reading
    // of the pressed value.
    bool isPressed = digitalRead(BUTTON3) == LOW;
    if (isPressed && isButton3Held) {
        return false;
    }
    isButton3Held = isPressed;
    return isPressed;  
}