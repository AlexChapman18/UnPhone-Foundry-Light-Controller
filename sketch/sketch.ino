/**
 * Main file to run application.
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
#include <private.h>

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ VARIABLE INITIALISATIONS ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

// Variables to setup the LCD and touchscreen
static const uint16_t screenWidth  = 320;
static const uint16_t screenHeight = 480;
TFT_eSPI tft = TFT_eSPI(screenWidth, screenHeight);
static lv_disp_draw_buf_t draw_buf;
static lv_color_t buf[screenWidth * 10];

// Initialise UnPhone
NuPhone nuphone = NuPhone();
// Initialise WiFi connection
ESPWiFi espwifi = ESPWiFi();
// Initialise ArtNetUniverse
ArtNetUniverse anu = ArtNetUniverse();
// Architecture groups
ArchitectureGroup architecture_group_list[15];
// Current architecure group the user is on
ArchitectureGroup *current_arc_group;

// WiFi Status updates for user interface
lv_obj_t *wifi_status_label;
// Used to detect a change
int current_wifi_status;

// Define the screens (pages)
static lv_obj_t *architecture_screen;      // screen 0
static lv_obj_t *color_screen;              // screen 1
static lv_obj_t *intensity_effects_screen;  // screen 2
static lv_obj_t *color_status_screen;       // screen 3
static int current_screen;                  // 0, 1, 2, or 3

// RGB sliders (global to allow live refresh)
static lv_obj_t *red_slider;
static lv_obj_t *green_slider;
static lv_obj_t *blue_slider;

// Colors list and corresponding RGB values
const char* colors_list[] = {"orange", "red", "rose", "magenta", "violet",
                             "blue", "azure", "cyan", "aquamarine", "green",
                             "chartreuse", "yellow", "White", "Off"};

const int color_values_list[][3] = {{255, 127, 0}, {255, 0, 0}, {255, 0, 127},
                                    {255, 0, 255}, {127, 0, 255}, {0, 0, 255},
                                    {0, 127, 255}, {0, 255, 255}, {0, 255, 127},
                                    {0, 255, 0}, {127, 255, 0}, {255, 255, 0},
                                    {255, 255, 255}, {0, 0, 0}};

// Architectures list
const char* architectures_list[] = {"Toilets", "Main\nExit", "Merch\nLights", "Pillars",
                                    "Dancefloor", "Main Bar\nLeft", "Main Bar\nRight",
                                    "Bar 1", "Bar 2", "Bar 3", "Raised\nArea Back", "Raised\nBar",
                                    "Raised\nFOH", "All\nArcs", "All\nBars"};
// Effect list
const char* effects_list[] = {"Solid", "Pulse\nEffect", "Odd-Even\nEffect",
                              "Fade\nSwipe", "Binary\nSwipe", "Bars\nFade"};


// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// ~~~~~~~~~~~~~~~~~~~~~~~ HELPER FUNCTIONS FOR SETTING UP LCD DISPLAY ~~~~~~~~~~~~~~~~~~~~~~~

/**
 * Map touch coordinates to LCD coordinates.
 * A version of map that never returns out of range values.
 * Function taken from Hamish's UnPhone examples.
 */
long myMapper(long x, long in_min, long in_max, long out_min, long out_max) {
    long probable = (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
    if(probable < out_min) return out_min;
    if(probable > out_max) return out_max;
    return probable;
}


/**
 * Read the touchpad.
 * Function taken from Hamish's UnPhone examples, with tweaks.
 */
void touchpadRead(lv_indev_drv_t * indev_driver, lv_indev_data_t * data) {
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


/**
 * Display flushing.
 * Function taken from Hamish's UnPhone examples.
 */
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

/**
 * Event listener for choosing the colors for an architecture.
 * @param e Contains all data about the event.
 */
static void evtHandlerColorBtns(lv_event_t *e) {
    lv_event_code_t code = lv_event_get_code(e);
    if(code == LV_EVENT_CLICKED) {
        // Retrieve the text of the button
        const char *text = lv_label_get_text(lv_obj_get_child(lv_event_get_target(e), NULL));
        // Find which exact button was pressed (for loop is compact rather than individual evt listeners)
        for (int i=0; i < sizeof(colors_list)/sizeof(colors_list[0]); i++) {
            if (strcmp(text, colors_list[i]) == 0) {
                current_arc_group->setRGB(color_values_list[i][0], color_values_list[i][1], color_values_list[i][2]);
                // Live update the sliders for user experience
                lv_slider_set_value(red_slider, color_values_list[i][0], LV_ANIM_ON);
                lv_slider_set_value(green_slider, color_values_list[i][1], LV_ANIM_ON);
                lv_slider_set_value(blue_slider, color_values_list[i][2], LV_ANIM_ON);
                break;
            }
        }
    }
}

/**
 * Event listener for choosing an architecture to change colors for.
 * @param e Contains all data about the event.
 */
static void evtHandlerArchGroupBtns(lv_event_t *e) {
    lv_event_code_t code = lv_event_get_code(e);
    if(code == LV_EVENT_CLICKED) {
        // Retrieve the text of the button
        const char *text = lv_label_get_text(lv_obj_get_child(lv_event_get_target(e), NULL));
        // Find which exact button was pressed (for loop is compact rather than individual evt listeners)
        for (int i=0; i < sizeof(architectures_list)/sizeof(architectures_list[0]); i++) {
            if (strcmp(text, architectures_list[i]) == 0) {
                renderColorScreen(&architecture_group_list[i]);
                lv_scr_load(color_screen);
                delete_previous_screen();
                current_screen = 1;
                break;
            }
        }
    }
}

/**
 * Event listener for choosing effects.
 * @param e Contains all data about the event.
 */
static void evtHandlerEffectsBtns(lv_event_t * e) {
    lv_event_code_t code = lv_event_get_code(e);
    if(code == LV_EVENT_CLICKED) {
        // Retrieve the text of the button
        const char *text = lv_label_get_text(lv_obj_get_child(lv_event_get_target(e), NULL));
        // Find which exact button was pressed (for loop is compact rather than individual evt listeners)
        for (int i=0; i < sizeof(effects_list)/sizeof(effects_list[0]); i++) {
            if (strcmp(text, effects_list[i]) == 0) {
                anu.current_effect = i;
                break;
            }
        }
    }
}

/**
 * Event listener for the back button to return to the architecture choice screen.
 * @param e Contains all data about the event.
 */
static void evtHandlerBackBtn(lv_event_t *e) {
    lv_event_code_t code = lv_event_get_code(e);
    if(code == LV_EVENT_CLICKED) {
        renderArchitectureScreen();
        lv_scr_load(architecture_screen);
        delete_previous_screen();
        current_screen = 0;
    }
}

/**
 * Event listener for the red color slider.
 * @param e Contains all data about the event.
 */
static void evtHandlerRedSlider(lv_event_t *e) {
    lv_obj_t *slider = lv_event_get_target(e);
    uint8_t value = (uint8_t)lv_slider_get_value(slider);
    current_arc_group->setRed(value);
}

/**
 * Event listener for the green color slider.
 * @param e Contains all data about the event.
 */
static void evtHandlerGreenSlider(lv_event_t *e) {
    lv_obj_t *slider = lv_event_get_target(e);
    uint8_t value = (uint8_t)lv_slider_get_value(slider);
    current_arc_group->setGreen(value);
}

/**
 * Event listener for the blue color slider.
 * @param e Contains all data about the event.
 */
static void evtHandlerBlueSlider(lv_event_t *e) {
    lv_obj_t *slider = lv_event_get_target(e);
    uint8_t value = (uint8_t)lv_slider_get_value(slider);
    current_arc_group->setBlue(value);
}

/**
 * Event listener for the intensity slider used in effects.
 * @param e Contains all data about the event.
 */
static void evtHandlerIntensitySlider(lv_event_t *e) {
    lv_obj_t *slider = lv_event_get_target(e);
    uint8_t value = (uint8_t)lv_slider_get_value(slider);
    // Bring value into range 0 and 1
    float normalised_value = (float)value / 255.0;
    anu.setIntensity(normalised_value);
}

/**
 * Event listener for the the speed slider used in effects.
 * @param e Contains all data about the event.
 */
static void evtHandlerSpeedSlider(lv_event_t *e) {
    lv_obj_t *slider = lv_event_get_target(e);
    uint8_t value = (uint8_t)lv_slider_get_value(slider);
    // Bring value into range 0 and 1
    float normalised_value = (float)value / 255.0;
    anu.setSpeed(normalised_value);
}

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ SCREENS (PAGES) ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

/**
 * Initialises and renders components onto the architecture screen.
*/
void renderArchitectureScreen() {
    architecture_screen = lv_obj_create(NULL);

    // Initalise WiFi status symbol
    wifiStatusHandler(architecture_screen);

    // Style object(s)
    static lv_style_t btn_style;

    // Define width, height and style for all btns on the page
    const int BUTTON_WIDTH = 90, BUTTON_HEIGHT = 75;
    lv_color_t BTN_BG_COLOR = LV_COLOR_MAKE(35, 35, 35);
    lv_coord_t BTN_ROUNDED = 5;

    int initial_x = 15, initial_y = 50;
    int PADDING_X = 10, PADDING_Y = 5;

    // Design the layout of the architecture screen
    createLabel(70, 20, "Select Architecture Group", architecture_screen);
    for (int i = 0; i < sizeof(architecture_group_list)/sizeof(architecture_group_list[0]); i++) {
        createButton(evtHandlerArchGroupBtns, initial_x, initial_y, BUTTON_WIDTH, BUTTON_HEIGHT, architectures_list[i],        
                     BTN_BG_COLOR, lv_color_white(), BTN_ROUNDED, &btn_style, architecture_screen);

        // Adjust coordinates as necessary for creating a grid
        initial_x += BUTTON_WIDTH + PADDING_X;
        if ((i + 1) % 3 == 0) {
            initial_x = 15;
            initial_y += BUTTON_HEIGHT + PADDING_Y;
        }
    }
}

/**
 * Initialises and renders components onto the color screen for a given architecuture.
 * @param current_group Architecture group user has selected to modify colors for.
*/
void renderColorScreen(ArchitectureGroup *current_group) {
    current_arc_group = current_group;
    color_screen = lv_obj_create(NULL);

    // Initalise WiFi status symbol
    wifiStatusHandler(color_screen);

    // Style object(s)
    static lv_style_t back_btn_style, black_btn_style, white_btn_style,
                      red_slider_style, green_slider_style, blue_slider_style;

    // The -2 in size is because we want to exclude black and white in the default color grid
    static lv_style_t color_styles_list[sizeof(colors_list) / sizeof(colors_list[0])-2];
    for (int i = 0; i < sizeof(colors_list) / sizeof(colors_list[0])-2; i++) {
        lv_style_init(&color_styles_list[i]);
    }

    // Define width, height and styles for components on the page
    const int BUTTON_WIDTH = 90, BUTTON_HEIGHT = 30;
    const int WHITE_AND_OFF_BUTTON_WIDTH = 70, WHITE_AND_OFF_BUTTON_HEIGHT = 80;
    const int SLIDER_WIDTH = 20, SLIDER_HEIGHT = 210;
    lv_coord_t COLOR_BTN_ROUNDED = 15;
    lv_coord_t REG_BTN_ROUNDED = 5;

    int initial_x = 20, initial_y = 55;
    int PADDING = 5;

    // Design the layout of the color screen
    createLabel(150, 13, "Lighting Color", color_screen);
    createButton(evtHandlerBackBtn, 20, 10, BUTTON_WIDTH, BUTTON_HEIGHT, "Back", lv_color_black(), lv_color_white(),
                 REG_BTN_ROUNDED, &back_btn_style, color_screen);

    for (int i = 0; i < sizeof(colors_list) / sizeof(colors_list[0])-2; i++) {
        lv_color_t color = LV_COLOR_MAKE(color_values_list[i][0], color_values_list[i][1], color_values_list[i][2]);
        createButton(evtHandlerColorBtns, initial_x, initial_y, BUTTON_WIDTH, BUTTON_HEIGHT, colors_list[i],
                     color, color, COLOR_BTN_ROUNDED, &color_styles_list[i], color_screen);

        // Adjust coordinates as necessary for creating a grid
        initial_x += BUTTON_WIDTH + PADDING;
        if ((i + 1) % 3 == 0) {
            initial_x = 20;
            initial_y += BUTTON_HEIGHT + PADDING;
        }
    }

    // Red slider and label
    red_slider = createSlider(evtHandlerRedSlider, 30, 220, SLIDER_WIDTH, SLIDER_HEIGHT, current_arc_group->getRed(),
                              LV_COLOR_MAKE(255, 0, 0), &red_slider_style, color_screen);
    createLabel(22, 450, "Red", color_screen);

    // Green slider and label
    green_slider = createSlider(evtHandlerGreenSlider, 102, 220, SLIDER_WIDTH, SLIDER_HEIGHT, current_arc_group->getGreen(),
                                LV_COLOR_MAKE(0, 200, 0), &green_slider_style, color_screen);
    createLabel(85, 450, "Green", color_screen);

    // Blue slider and label
    blue_slider = createSlider(evtHandlerBlueSlider, 180, 220, SLIDER_WIDTH, SLIDER_HEIGHT, current_arc_group->getBlue(),
                               LV_COLOR_MAKE(0, 0, 255), &blue_slider_style, color_screen);
    createLabel(170, 450, "Blue", color_screen);

    // White light button
    createButton(evtHandlerColorBtns, 230, 220, WHITE_AND_OFF_BUTTON_WIDTH, WHITE_AND_OFF_BUTTON_HEIGHT, colors_list[12],
                 lv_color_white(), lv_color_black(), REG_BTN_ROUNDED, &white_btn_style, color_screen);

    // Off button
    createButton(evtHandlerColorBtns, 230, 320, WHITE_AND_OFF_BUTTON_WIDTH, WHITE_AND_OFF_BUTTON_HEIGHT, colors_list[13],
                 lv_color_black(), lv_color_white(), REG_BTN_ROUNDED, &black_btn_style, color_screen);
}


/**
 * Initialises and renders components onto the intensity and effects screen
 * for a given architecuture.
*/
void renderIntensityEffectsScreen() {
    intensity_effects_screen = lv_obj_create(NULL);

    // Initalise WiFi status symbol
    wifiStatusHandler(intensity_effects_screen);

    // Style object(s)
    static lv_style_t effect_button_style, solid_button_style, back_btn_style, slider_style;

    // Define width, height and styles for components on the page
    const int BUTTON_WIDTH = 90, BUTTON_HEIGHT = 55;
    const int SLIDER_WIDTH = 20, SLIDER_HEIGHT = 280;
    lv_coord_t BTN_ROUNDED = 5;
    lv_color_t BTN_BG_COLOR = LV_COLOR_MAKE(45, 45, 45);
    lv_color_t BTN_TEXT_COLOR = LV_COLOR_MAKE(255, 255, 255);

    int initial_y = 70;
    int PADDING = 5;

    // Design the layout of the color screen
    for (int i = 0; i < sizeof(effects_list) / sizeof(effects_list[0]); i++) {
        createButton(evtHandlerEffectsBtns, 210, initial_y, BUTTON_WIDTH, BUTTON_HEIGHT, effects_list[i],
                     BTN_BG_COLOR, BTN_TEXT_COLOR, BTN_ROUNDED, &solid_button_style, intensity_effects_screen);
        
        initial_y += BUTTON_HEIGHT + PADDING;
    }

    // Denormalise intensity and speed
    uint8_t denormalised_intensity_value = (uint8_t)(anu.getIntensity()*255.0);
    uint8_t denormalised_speed_value = (uint8_t)(anu.getSpeed()*255.0);

    // Brightness / Intensity slider
    createSlider(evtHandlerIntensitySlider, 40, 100, SLIDER_WIDTH, SLIDER_HEIGHT, denormalised_intensity_value, BTN_BG_COLOR,
                 &slider_style, intensity_effects_screen);
    createLabel(20, 405, "Intensity", intensity_effects_screen);

    // Effect spped slider
    createSlider(evtHandlerSpeedSlider, 135, 100, SLIDER_WIDTH, SLIDER_HEIGHT, denormalised_speed_value, BTN_BG_COLOR,
                 &slider_style, intensity_effects_screen);
    createLabel(120, 405, "Effect\nSpeed", intensity_effects_screen);
}


/**
 * Initialises and renders components onto the global color screen
 * (visualise all arc colors on one screen).
*/
void renderColorStatusScreen() {
    color_status_screen = lv_obj_create(NULL);

    // Initalise WiFi status symbol
    wifiStatusHandler(color_status_screen);

    // Style object(s)
    static lv_style_t styles_list[sizeof(architecture_group_list)/sizeof(architecture_group_list[0])];
    for (int i = 0; i < sizeof(architecture_group_list)/sizeof(architecture_group_list[0]); i++) {
        lv_style_init(&styles_list[i]);
    }
                
    // Define width, height and styles
    const int COLOR_BLOCK_WIDTH = 50, COLOR_BLOCK_HEIGHT = 20;
    lv_coord_t COLOR_BLOCK_ROUNDED = 5;
    int initial_x = 30, initial_y = 60;
    int PADDING = 5;

    // Design the layout of the screen
    createLabel(88, 13, "Global Color Status", color_status_screen);
    for (int i=0; i < sizeof(architecture_group_list)/sizeof(architecture_group_list[0]); i++) {
        // Get the name of the architecture and remove "\n" and replace with a space
        const char* name = architecture_group_list[i].getName();
        std::string nameString = std::string(name);
        std::replace(nameString.begin(), nameString.end(), '\n', ' ');

        // Create the RGB color
        lv_color_t bg_color = LV_COLOR_MAKE(architecture_group_list[i].getRed(),
                                            architecture_group_list[i].getGreen(),
                                            architecture_group_list[i].getBlue());

        // Add the architecture label with the corresponding background color
        createLabel(initial_x, initial_y, nameString.c_str(), color_status_screen);
        createRectangle(initial_x+200, initial_y, COLOR_BLOCK_WIDTH, COLOR_BLOCK_HEIGHT,
                        bg_color, COLOR_BLOCK_ROUNDED, &styles_list[i], color_status_screen);

        initial_y += COLOR_BLOCK_HEIGHT + PADDING;
    }
}


/**
 * Used on each screen to initialise the WiFi status.
*/
void wifiStatusHandler(lv_obj_t *screen) {
  wifi_status_label = createLabel(290, 10, "", screen);
  lv_label_set_text(wifi_status_label, espwifi.isConnected() ? LV_SYMBOL_WIFI : LV_SYMBOL_CLOSE);
}


/**
 * Used in screen switching.
 * Deletes the screen after the user has switched to another screen.
*/
void delete_previous_screen() {
  if (current_screen == 0) {lv_obj_del(architecture_screen); }
  if (current_screen == 1) {lv_obj_del(color_screen); }
  if (current_screen == 2) {lv_obj_del(intensity_effects_screen); }
  if (current_screen == 3) {lv_obj_del(color_status_screen); }
}


// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ SETUP AND LOOP ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

void setup() {
  Serial.begin(115200);

  // Begin unPhone with a set orientation
  nuphone.begin();
  nuphone.tsp->setRotation(2);
  nuphone.setBacklight(true);

  // Begin WiFi and ArtNetUniverse
  espwifi.begin();
  anu.setup();

  // Initiate the LCD
  tft.begin();
  tft.setRotation(0);

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
      architecture_group_list[i] = ArchitectureGroup(i, architectures_list[i]);
  }

  // Render and load the initial screen
  renderArchitectureScreen();
  lv_scr_load(architecture_screen);
  current_screen = 0;
  current_wifi_status = espwifi.isConnected();

  // Begin art-net transmission
  anu.begin();
}

void loop() { 
    lv_timer_handler();
    
    // Switch screens if UnPhone buttons are pressed
    if (nuphone.isButton1()) {
      if (current_screen != 0) {
        renderArchitectureScreen();
        lv_scr_load(architecture_screen);
        delete_previous_screen();
        current_screen = 0;
      }
    } else if (nuphone.isButton2()) {
      if (current_screen != 2) {
        renderIntensityEffectsScreen();
        lv_scr_load(intensity_effects_screen);
        delete_previous_screen();
        current_screen = 2;
      }
    } else if (nuphone.isButton3()) {
      if (current_screen != 3) {
        renderColorStatusScreen();
        lv_scr_load(color_status_screen);
        delete_previous_screen();
        current_screen = 3;
      }
    }
    // Check if there is a change in the WiFi connection
    if (current_wifi_status != espwifi.isConnected()) {
      lv_label_set_text(wifi_status_label, espwifi.isConnected() ? LV_SYMBOL_WIFI : LV_SYMBOL_CLOSE);
      current_wifi_status = espwifi.isConnected();
    }
    delay(5);
}
