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

// static ref to singleton; only use after constr!
NuPhone *NuPhone::me = NULL;

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

    // Setup backlight:
    NuPhone::initializeTCAChip();

    // Setup touchscreen
    tsp = new XPT2046_Touchscreen(TOUCH_CS); // no IRQ

    // start power switch checking
    xTaskCreate(continuousPowerCheck, "power switch task", 4096, NULL, 1, NULL);
    tsp->begin();
}

void NuPhone::initializeTCAChip(){
    Wire.begin();
    Wire.beginTransmission(TCA9555_ADDRESS); // Transmit to the TCA chip
    Wire.write(0x06); // Talk to Configuration Registers 1
    Wire.write(0b11111011); // Set the backlight as an output pin
    Wire.write(0xFF); // Set the other pins to Input
    Wire.endTransmission();
}

// Check the power switch and turn the device off if necissary
void NuPhone::checkPowerSwitch() {
    if (!digitalRead(POWER_SWITCH)) {  // Is the switch in the off position?
        // If so, enable "Wake on power switch" and go to sleep
        NuPhone::setBacklight(false);
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
    Wire.beginTransmission(TCA9555_ADDRESS); // Transmit to the TCA Chip
    Wire.write(0x02); // Specify the Output Port Register 1
    if (shouldBacklight) Wire.write(0b00000100); // Turn on the backlight output pin
    else Wire.write(0b00000000); // Turn off the backlight output pin
    Wire.endTransmission();
}

void continuousPowerCheck(void *param) {
  // Check power switch every 10th of sec
  while (true) { NuPhone::me->checkPowerSwitch(); delay(100); }
}

bool NuPhone::isButton1() { 
    bool isPressed = digitalRead(BUTTON1) == LOW;
    if (isPressed && isButton1Held) {
        return false;
    }
    isButton1Held = isPressed;
    return isPressed; 
}

bool NuPhone::isButton2() { 
    bool isPressed = digitalRead(BUTTON2) == LOW;
    if (isPressed && isButton2Held) {
        return false;
    }
    isButton1Held = isPressed;
    return isPressed; 
}

bool NuPhone::isButton3() { 
    bool isPressed = digitalRead(BUTTON3) == LOW;
    if (isPressed && isButton3Held) {
        return false;
    }
    isButton1Held = isPressed;
    return isPressed;  
}