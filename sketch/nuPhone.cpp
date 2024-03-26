/**
 * A reduced version of NuPhone.cpp
 * 
 * Author: Kush Bharakhada and Alex Chapman
 * NuPhone.cpp
*/

#include <NuPhone.h>

#define POWER_SWITCH 18
#define LED_RED 13
#define LED_GREEN 9
#define LED_BLUE 13
static const uint8_t BUTTON1 = 45;   // left button
static const uint8_t BUTTON2 = 0;   // middle button
static const uint8_t BUTTON3 = 21;   // right button

// LCD Pins
static const uint8_t BACKLIGHT = 2 | 0x40;
static const uint8_t LCD_CS = 48;
static const uint8_t LCD_DC = 47;
static const uint8_t TOUCH_CS = 38;
static const uint8_t LCD_RESET = 46;


// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ NUPHONE CLASS ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

NuPhone::NuPhone(){}

void NuPhone::begin() {
    // Setup RGB LED
    pinMode(LED_RED,   OUTPUT);
    pinMode(LED_GREEN, OUTPUT);
    pinMode(LED_BLUE,  OUTPUT);
    setLEDRGB(0, 0, 0);

    // Setup buttons
    pinMode(BUTTON1, INPUT_PULLUP);
    pinMode(BUTTON2, INPUT_PULLUP);
    pinMode(BUTTON3, INPUT_PULLUP);
}

// Check the power switch and turn the device off if necissary
void NuPhone::checkPowerSwitch() {
    // Is the switch in the off position?
    if (! digitalRead(POWER_SWITCH)) {
        // If so, enable "Wake on power switch" and go to sleep
        esp_sleep_enable_ext0_wakeup((gpio_num_t) POWER_SWITCH, 1);
        esp_deep_sleep_start();
    }
}

// Sets the color of the LED on the NuPhone
void NuPhone::setLEDRGB(uint8_t red, uint8_t green, uint8_t blue) {
    red = !red; green = !green; blue = !blue;
    digitalWrite(LED_RED, red);
    digitalWrite(LED_GREEN, green);
    digitalWrite(LED_BLUE, blue);
}

void NuPhone::setBacklight(bool shouldBacklight) {
    if(shouldBacklight) digitalWrite(BACKLIGHT, HIGH);
    else digitalWrite(BACKLIGHT, LOW);
}