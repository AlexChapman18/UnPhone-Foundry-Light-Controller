/**
 * Main file.
 * 
 * Author: Kush Bharakhada and Alex Chapman
 * sketch.ino
*/

#include <wifi_utils.h>
#include <artnet_utils.h>
#include <fixture_utils.h>
#include <NuPhone.h>
#include <TFT_eSPI.h>
#include <LVGL_utils.h>
#include <Adafruit_SPIFlash.h>

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ VARIABLE INITIALISATIONS ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

// Variables to setup the LCD and touchscreen
static const uint16_t screenWidth  = 320;
static const uint16_t screenHeight = 480;
TFT_eSPI tft = TFT_eSPI(screenWidth, screenHeight);
static lv_disp_draw_buf_t draw_buf;
static lv_color_t buf[ screenWidth * 10 ];

// Define the screens
static lv_obj_t *architectural_screen;
static lv_obj_t *color_screen;
static lv_obj_t *intensity_effects_screen;
static lv_obj_t *current_screen;

// Initialise UnPhone
NuPhone nuphone = NuPhone();

// Initialise WiFi connection
ESPWiFi espwifi = ESPWiFi();

// Initialise ArtNetUniverse
ArtNetUniverse anu = ArtNetUniverse();

// Architecture groups
ArchitectureGroup architecture_group_list[15];
// Address list          
uint8_t address_list[] = {1, 4, 7};

// Colors list
const char* colors_list[] = {"orange", "red", "rose", "magenta", "violet", "blue", "azure", "cyan", "aquamarine",
                             "green", "chartreuse", "yellow", "White", "Off"};

const int color_values_list[][3] = {{255, 127, 0}, {255, 0, 0}, {255, 0, 127}, {255, 0, 255}, {127, 0, 255}, {0, 0, 255},
                                    {0, 127, 255}, {0, 255, 255}, {0, 255, 127}, {0, 255, 0}, {127, 255, 0}, {255, 255, 0},
                                    {255, 255, 255}, {0, 0, 0}};

// Architectures list
const char* architectures_list[] = {"Toilets", "Main\nExit", "Merch\nLights", "Pillars", "Dancefloor", "Main Bar\nLeft",
                                    "Main Bar\nRight", "Bar 1", "Bar 2", "Bar 3", "Raised\nArea Back", "Raised\nBar",
                                    "Raised\nFOH", "All\nArcs", "All\nBars"};
// Effect list          
const char* effects_list[] = {"Solid", "Pulse\nEffect", "Odd-Even\nEffect", "Swiping\nEffect", "Temp1", "Temp2"};

ArchitectureGroup *currentArcGroup;

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// ~~~~~~~~~~~~~~~~~~~~~~~~ HELPER FUNCTIONS FOR SETTING UP LCD DISPLAY ~~~~~~~~~~~~~~~~~~~~~~

long myMapper(long x, long in_min, long in_max, long out_min, long out_max) {
  // Map touch coordinates to LCD coordinates. A version of map that never returns out of range values.
  long probable = (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
  if(probable < out_min) return out_min;
  if(probable > out_max) return out_max;
  return probable;
}

void touchpadRead(lv_indev_drv_t * indev_driver, lv_indev_data_t * data) {
  // Read the touchpad.
  uint16_t touchX, touchY;
  bool touched = nuphone.tsp->touched();

  if (!touched) {
    data->state = LV_INDEV_STATE_REL;
  } 
  else {
    data->state = LV_INDEV_STATE_PR;

    // Set the coordinates
    TS_Point p(-1, -1, -1);
    p = nuphone.tsp->getPoint();

    if(p.z < 400) { return; }
    if(p.x < 0 || p.y < 0) Serial.println("Less than zero!");

    long xMin = 320, xMax = 3945;
    long yMin = 220, yMax = 3915;

    long xscld = myMapper((long) p.x, xMin, xMax, 0, (long) screenWidth);
    long yscld = ((long) screenHeight) - myMapper((long) p.y, yMin, yMax, 0, (long) screenHeight);

    touchX = (uint16_t) xscld;
    touchY = (uint16_t) yscld;
    data->point.x = touchX;
    data->point.y = touchY;
  }
}

void displayFlush(lv_disp_drv_t *disp, const lv_area_t *area, lv_color_t *color_p) {
  uint32_t w = (area->x2 - area->x1 + 1);
  uint32_t h = (area->y2 - area->y1 + 1);
  tft.startWrite();
  tft.setAddrWindow(area->x1, area->y1, w, h);
  tft.pushColors((uint16_t*)&color_p->full, w * h, true);
  tft.endWrite();
  lv_disp_flush_ready(disp);
}

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ EVENT LISTENERS ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

static void evtHandlerColorBtns(lv_event_t * e) {
    lv_event_code_t code = lv_event_get_code(e);
    if(code == LV_EVENT_CLICKED) {
        // Retrieve the text of the button
        const char *text = lv_label_get_text(lv_obj_get_child(lv_event_get_target(e), NULL));
        // Find which exact button was pressed (for loop is compact rather than individual evt listeners)
        for (int i=0; i < sizeof(colors_list)/sizeof(colors_list[0]); i++) {
            if (strcmp(text, colors_list[i]) == 0) {
                currentArcGroup->setRGB(color_values_list[i][0], color_values_list[i][1], color_values_list[i][2]);
                break;
            }
        }
    }
}

static void evtHandlerArchGroupBtns(lv_event_t * e) {
    lv_event_code_t code = lv_event_get_code(e);
    if(code == LV_EVENT_CLICKED) {
        // Retrieve the text of the button
        const char *text = lv_label_get_text(lv_obj_get_child(lv_event_get_target(e), NULL));
        // Find which exact button was pressed (for loop is compact rather than individual evt listeners)
        for (int i=0; i < sizeof(architectures_list)/sizeof(architectures_list[0]); i++) {
            if (strcmp(text, architectures_list[i]) == 0) {
                switchToColorScreen(&architecture_group_list[i]);
                break;
            }
        }
    }
}

static void evtHandlerEffectsBtns(lv_event_t * e) {
    lv_event_code_t code = lv_event_get_code(e);
    if(code == LV_EVENT_CLICKED) {
        // Retrieve the text of the button
        const char *text = lv_label_get_text(lv_obj_get_child(lv_event_get_target(e), NULL));
        // Find which exact button was pressed (for loop is compact rather than individual evt listeners)
        for (int i=0; i < sizeof(effects_list)/sizeof(effects_list[0]); i++) {
            if (strcmp(text, effects_list[i]) == 0) {
                Serial.println(effects_list[i]);
                break;
            }
        }
    }
}

static void evtHandlerBackBtn(lv_event_t * e) {
    lv_event_code_t code = lv_event_get_code(e);
    if(code == LV_EVENT_CLICKED) {
        switchToArchitecturalScreen();
    }
}

static void evtHandlerRedSlider(lv_event_t * e) {
    lv_obj_t * slider = lv_event_get_target(e);
    uint8_t value = (uint8_t)lv_slider_get_value(slider);
    currentArcGroup->setRed(value);
}

static void evtHandlerGreenSlider(lv_event_t * e) {
    lv_obj_t * slider = lv_event_get_target(e);
    uint8_t value = (uint8_t)lv_slider_get_value(slider);
    currentArcGroup->setGreen(value);
}

static void evtHandlerBlueSlider(lv_event_t * e) {
    lv_obj_t * slider = lv_event_get_target(e);
    uint8_t value = (uint8_t)lv_slider_get_value(slider);
    currentArcGroup->setBlue(value);
}

static void evtHandlerIntensitySlider(lv_event_t * e) {
    lv_obj_t * slider = lv_event_get_target(e);
    uint8_t value = (uint8_t)lv_slider_get_value(slider);
    // Bring value into range 0 and 1
    float normalised_value = (float)value / 255.0;
    anu.setIntensity(normalised_value);
}

static void evtHandlerSpeedSlider(lv_event_t * e) {
    lv_obj_t * slider = lv_event_get_target(e);
    uint8_t value = (uint8_t)lv_slider_get_value(slider);
    // Bring value into range 0 and 1
    float normalised_value = (float)value / 255.0;
    anu.setSpeed(normalised_value);
}

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ SCREENS (PAGES) ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

void renderArchitecturalScreen() {
    architectural_screen = lv_obj_create(NULL);

    // Style object(s)
    static lv_style_t btn_style;

    // Define width, height and style for all btns on the page
    const int BUTTON_WIDTH = 90, BUTTON_HEIGHT = 75;
    lv_color_t btn_bg_color = LV_COLOR_MAKE(35, 35, 35);
    lv_coord_t btn_rounded = 5;

    // Design the layout of the architectural screen
    createLabel(70, 20, "Select Architectural Group", architectural_screen);
    createButton(evtHandlerArchGroupBtns, 20,  50, BUTTON_WIDTH, BUTTON_HEIGHT, "Toilets",        
                 btn_bg_color, lv_color_white(), btn_rounded, &btn_style, architectural_screen);
    createButton(evtHandlerArchGroupBtns, 115, 50, BUTTON_WIDTH, BUTTON_HEIGHT, "Main\nExit",
                 btn_bg_color, lv_color_white(), btn_rounded, &btn_style, architectural_screen);
    createButton(evtHandlerArchGroupBtns, 210, 50, BUTTON_WIDTH, BUTTON_HEIGHT, "Merch\nLights",
                 btn_bg_color, lv_color_white(), btn_rounded, &btn_style, architectural_screen);
    createButton(evtHandlerArchGroupBtns, 20,  130, BUTTON_WIDTH, BUTTON_HEIGHT, "Pillars",
                 btn_bg_color, lv_color_white(), btn_rounded, &btn_style, architectural_screen);
    createButton(evtHandlerArchGroupBtns, 115, 130, BUTTON_WIDTH, BUTTON_HEIGHT, "Dancefloor",
                 btn_bg_color, lv_color_white(), btn_rounded, &btn_style, architectural_screen);
    createButton(evtHandlerArchGroupBtns, 210, 130, BUTTON_WIDTH, BUTTON_HEIGHT, "Main Bar\nLeft",
                 btn_bg_color, lv_color_white(), btn_rounded, &btn_style, architectural_screen);
    createButton(evtHandlerArchGroupBtns, 20,  210, BUTTON_WIDTH, BUTTON_HEIGHT, "Main Bar\nRight",
                 btn_bg_color, lv_color_white(), btn_rounded, &btn_style, architectural_screen);
    createButton(evtHandlerArchGroupBtns, 115, 210, BUTTON_WIDTH, BUTTON_HEIGHT, "Bar 1",
                 btn_bg_color, lv_color_white(), btn_rounded, &btn_style, architectural_screen);
    createButton(evtHandlerArchGroupBtns, 210, 210, BUTTON_WIDTH, BUTTON_HEIGHT, "Bar 2",
                 btn_bg_color, lv_color_white(), btn_rounded, &btn_style, architectural_screen);
    createButton(evtHandlerArchGroupBtns, 20,  290, BUTTON_WIDTH, BUTTON_HEIGHT, "Bar 3",
                 btn_bg_color, lv_color_white(), btn_rounded, &btn_style, architectural_screen);
    createButton(evtHandlerArchGroupBtns, 115, 290, BUTTON_WIDTH, BUTTON_HEIGHT, "Raised\nArea Back",
                 btn_bg_color, lv_color_white(), btn_rounded, &btn_style, architectural_screen);
    createButton(evtHandlerArchGroupBtns, 210, 290, BUTTON_WIDTH, BUTTON_HEIGHT, "Raised\nBar",
                 btn_bg_color, lv_color_white(), btn_rounded, &btn_style, architectural_screen);
    createButton(evtHandlerArchGroupBtns, 20,  370, BUTTON_WIDTH, BUTTON_HEIGHT, "Raised\nFOH",
                 btn_bg_color, lv_color_white(), btn_rounded, &btn_style, architectural_screen);
    createButton(evtHandlerArchGroupBtns, 115, 370, BUTTON_WIDTH, BUTTON_HEIGHT, "All\nArcs",
                 btn_bg_color, lv_color_white(), btn_rounded, &btn_style, architectural_screen);
    createButton(evtHandlerArchGroupBtns, 210, 370, BUTTON_WIDTH, BUTTON_HEIGHT, "All\nBars",
                 btn_bg_color, lv_color_white(), btn_rounded, &btn_style, architectural_screen);
}

void switchToArchitecturalScreen() {
    renderArchitecturalScreen();
    lv_scr_load(architectural_screen);
    lv_obj_del(current_screen);
    current_screen = architectural_screen;
}

void switchToColorScreen(ArchitectureGroup * currentGroup) {
    currentArcGroup = currentGroup;
    color_screen = lv_obj_create(NULL);

    // Style object(s)
    static lv_style_t back_btn_style, orange_btn_style, red_btn_style, rose_btn_style, magenta_btn_style, 
                      violet_btn_style, blue_btn_style, azure_btn_style, cyan_btn_style, aquamarine_btn_style,
                      green_btn_style, chartreuse_btn_style, yellow_btn_style, black_btn_style, white_btn_style,
                      red_slider_style, green_slider_style, blue_slider_style;

    // Define width, height and styles for components on the page
    const int BUTTON_WIDTH = 90, BUTTON_HEIGHT = 30;
    const int WHITE_AND_OFF_BUTTON_WIDTH = 70, WHITE_AND_OFF_BUTTON_HEIGHT = 80;
    const int SLIDER_WIDTH = 20, SLIDER_HEIGHT = 210;
    lv_coord_t color_btn_rounded = 15;
    lv_coord_t btn_rounded = 5;

    // Design the layout of the color screen
    createLabel(200, 13, "Lighting Color", color_screen);
    createButton(evtHandlerBackBtn, 20, 10, BUTTON_WIDTH, BUTTON_HEIGHT, "Back",
                 lv_color_black(), lv_color_white(), btn_rounded, &back_btn_style, color_screen);
    createButton(evtHandlerColorBtns, 20, 55, BUTTON_WIDTH, BUTTON_HEIGHT, colors_list[0],
                 LV_COLOR_MAKE(255,  127, 0), LV_COLOR_MAKE(255, 127, 0), color_btn_rounded, &orange_btn_style, color_screen);
    createButton(evtHandlerColorBtns, 115, 55, BUTTON_WIDTH, BUTTON_HEIGHT, colors_list[1],
                 LV_COLOR_MAKE(255, 0, 0), LV_COLOR_MAKE(255, 0, 0), color_btn_rounded, &red_btn_style, color_screen);
    createButton(evtHandlerColorBtns, 210, 55, BUTTON_WIDTH, BUTTON_HEIGHT, colors_list[2],
                 LV_COLOR_MAKE(255, 0, 127), LV_COLOR_MAKE(255, 0, 127), color_btn_rounded, &rose_btn_style, color_screen);
    createButton(evtHandlerColorBtns, 20, 90, BUTTON_WIDTH, BUTTON_HEIGHT, colors_list[3],
                 LV_COLOR_MAKE(255, 0, 255), LV_COLOR_MAKE(255, 0, 255), color_btn_rounded, &magenta_btn_style, color_screen);
    createButton(evtHandlerColorBtns, 115, 90, BUTTON_WIDTH, BUTTON_HEIGHT, colors_list[4],
                 LV_COLOR_MAKE(127, 0, 255), LV_COLOR_MAKE(127, 0, 255), color_btn_rounded, &violet_btn_style, color_screen);
    createButton(evtHandlerColorBtns, 210, 90, BUTTON_WIDTH, BUTTON_HEIGHT, colors_list[5],
                 LV_COLOR_MAKE(0, 0, 255), LV_COLOR_MAKE(0, 0, 255), color_btn_rounded, &blue_btn_style, color_screen);
    createButton(evtHandlerColorBtns, 20, 125, BUTTON_WIDTH, BUTTON_HEIGHT, colors_list[6],
                 LV_COLOR_MAKE(0, 127, 255), LV_COLOR_MAKE(0, 127, 255), color_btn_rounded, &azure_btn_style, color_screen);
    createButton(evtHandlerColorBtns, 115, 125, BUTTON_WIDTH, BUTTON_HEIGHT, colors_list[7], 
                 LV_COLOR_MAKE(0, 255, 255), LV_COLOR_MAKE(0, 255, 255), color_btn_rounded, &cyan_btn_style, color_screen);
    createButton(evtHandlerColorBtns, 210, 125, BUTTON_WIDTH, BUTTON_HEIGHT, colors_list[8],
                 LV_COLOR_MAKE(0, 255, 127), LV_COLOR_MAKE(0, 255, 127), color_btn_rounded, &aquamarine_btn_style, color_screen);
    createButton(evtHandlerColorBtns, 20, 160, BUTTON_WIDTH, BUTTON_HEIGHT, colors_list[9],
                 LV_COLOR_MAKE(0, 255, 0), LV_COLOR_MAKE(0, 255, 0), color_btn_rounded, &green_btn_style, color_screen);
    createButton(evtHandlerColorBtns, 115, 160, BUTTON_WIDTH, BUTTON_HEIGHT, colors_list[10],
                 LV_COLOR_MAKE(127, 255, 0), LV_COLOR_MAKE(127, 255, 0), color_btn_rounded, &chartreuse_btn_style, color_screen);
    createButton(evtHandlerColorBtns, 210, 160, BUTTON_WIDTH, BUTTON_HEIGHT, colors_list[11],
                 LV_COLOR_MAKE(255, 255, 0), LV_COLOR_MAKE(255, 255, 0), color_btn_rounded, &yellow_btn_style, color_screen);

    // Red slider and label
    createSlider(evtHandlerRedSlider, 30, 220, SLIDER_WIDTH, SLIDER_HEIGHT, currentArcGroup->getRed(),
                 LV_COLOR_MAKE(255, 0, 0), &red_slider_style, color_screen);
    createLabel(22, 450, "Red", color_screen);

    // Green slider and label
    createSlider(evtHandlerGreenSlider, 102, 220, SLIDER_WIDTH, SLIDER_HEIGHT, currentArcGroup->getGreen(),
                 LV_COLOR_MAKE(0, 200, 0), &green_slider_style, color_screen);
    createLabel(85, 450, "Green", color_screen);

    // Blue slider and label
    createSlider(evtHandlerBlueSlider, 180, 220, SLIDER_WIDTH, SLIDER_HEIGHT, currentArcGroup->getBlue(),
                 LV_COLOR_MAKE(0, 0, 255), &blue_slider_style, color_screen);
    createLabel(170, 450, "Blue", color_screen);

    // White light button
    createButton(evtHandlerColorBtns, 230, 220, WHITE_AND_OFF_BUTTON_WIDTH, WHITE_AND_OFF_BUTTON_HEIGHT, colors_list[12],
                 lv_color_white(), lv_color_black(), btn_rounded, &white_btn_style, color_screen);

    // Off button
    createButton(evtHandlerColorBtns, 230, 320, WHITE_AND_OFF_BUTTON_WIDTH, WHITE_AND_OFF_BUTTON_HEIGHT, colors_list[13],
                 lv_color_black(), lv_color_white(), btn_rounded, &black_btn_style, color_screen);
    
    lv_scr_load(color_screen);
    lv_obj_del(current_screen);
    current_screen = color_screen;
}

void switchToIntensityEffectsScreen() {
    intensity_effects_screen = lv_obj_create(NULL);

    // Style object(s)
    static lv_style_t effect_button_style, solid_button_style, back_btn_style, slider_style;

    // Define width, height and styles for components on the page
    const int BUTTON_WIDTH = 90, BUTTON_HEIGHT = 55;
    const int SLIDER_WIDTH = 20, SLIDER_HEIGHT = 280;
    lv_coord_t btn_rounded = 5;
    lv_color_t bg_color = LV_COLOR_MAKE(45, 45, 45);
    lv_color_t btn_text_color = LV_COLOR_MAKE(255, 255, 255);

    // Design the layout of the color screen
    createLabel(85, 13, "Intensity and Effects", intensity_effects_screen);
    createButton(evtHandlerEffectsBtns, 210, 70, BUTTON_WIDTH, BUTTON_HEIGHT, effects_list[0],
                 lv_color_black(), btn_text_color, btn_rounded, &solid_button_style, intensity_effects_screen);
    createButton(evtHandlerEffectsBtns, 210, 130, BUTTON_WIDTH, BUTTON_HEIGHT, effects_list[1],
                 bg_color, btn_text_color, btn_rounded, &effect_button_style, intensity_effects_screen);
    createButton(evtHandlerEffectsBtns, 210, 190, BUTTON_WIDTH, BUTTON_HEIGHT, effects_list[2],
                 bg_color, btn_text_color, btn_rounded, &effect_button_style, intensity_effects_screen);
    createButton(evtHandlerEffectsBtns, 210, 250, BUTTON_WIDTH, BUTTON_HEIGHT, effects_list[3],
                 bg_color, btn_text_color, btn_rounded, &effect_button_style, intensity_effects_screen);
    createButton(evtHandlerEffectsBtns, 210, 310, BUTTON_WIDTH, BUTTON_HEIGHT, effects_list[4],
                 bg_color, btn_text_color, btn_rounded, &effect_button_style, intensity_effects_screen);
    createButton(evtHandlerEffectsBtns, 210, 370, BUTTON_WIDTH, BUTTON_HEIGHT, effects_list[5],
                 bg_color, btn_text_color, btn_rounded, &effect_button_style, intensity_effects_screen);
    
    // Denormalise intensity and speed
    uint8_t denormalised_intensity_value = (uint8_t)(anu.getIntensity()*255.0);
    uint8_t denormalised_speed_value = (uint8_t)(anu.getSpeed()*255.0);

    // Brightness / Intensity slider
    createSlider(evtHandlerIntensitySlider, 40, 100, SLIDER_WIDTH, SLIDER_HEIGHT, denormalised_intensity_value, bg_color,
                 &slider_style, intensity_effects_screen);
    createLabel(20, 405, "Intensity", intensity_effects_screen);

    // Effect spped slider
    createSlider(evtHandlerSpeedSlider, 135, 100, SLIDER_WIDTH, SLIDER_HEIGHT, denormalised_speed_value, bg_color,
                 &slider_style, intensity_effects_screen);
    createLabel(120, 405, "Effect\nSpeed", intensity_effects_screen);

    lv_scr_load(intensity_effects_screen);
    lv_obj_del(current_screen);
    current_screen = intensity_effects_screen;
}


// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ SETUP AND LOOP ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

void setup() {
  Serial.begin(115200);
  // Wait for Serial line to open
  while (!Serial) { ; }

  // Begin unPhone with a set orientation
  nuphone.begin();
  nuphone.tsp->setRotation(2);
  nuphone.setBacklight(true);

  // Begin WiFi
  espwifi.begin();

  // Begin ArtNetUniverse
  anu.setup();

  // Initiate the LCD
  tft.begin();
  tft.setRotation(0);

  // TODO: Calibration needs changing
  uint16_t calData[5] = { 347, 3549, 419, 3352, 5 };
  tft.setTouch(calData);

  // Initiate LVGL and initialise the drawing buffer
  lv_init();
  lv_disp_draw_buf_init( &draw_buf, buf, NULL, screenWidth * 10 );

  // Initialise the display driver
  static lv_disp_drv_t disp_drv;
  lv_disp_drv_init(&disp_drv);
  disp_drv.hor_res = screenWidth;
  disp_drv.ver_res = screenHeight;
  disp_drv.flush_cb = displayFlush;
  disp_drv.draw_buf = &draw_buf;
  lv_disp_drv_register(&disp_drv);

  // Initialise the input device driver 
  static lv_indev_drv_t indev_drv;
  lv_indev_drv_init(&indev_drv);
  indev_drv.type = LV_INDEV_TYPE_POINTER;
  indev_drv.read_cb = touchpadRead;
  lv_indev_drv_register(&indev_drv);

  // Create architecture objects
  for (int i=0; i < sizeof(architecture_group_list)/sizeof(architecture_group_list[0]); i++) {
      architecture_group_list[i] = ArchitectureGroup(address_list, 3, architectures_list[i]);
  }

  // Render and load the initial screen
  renderArchitecturalScreen();
  lv_scr_load(architectural_screen);
  current_screen = architectural_screen;

//   Begin art-net transmissiom
    anu.begin();
}

void loop() { 
    lv_timer_handler(); 
    if (nuphone.isButton1()) {

    }
    if (nuphone.isButton2()) {

    }
    if (nuphone.isButton3()) {

    }
}
