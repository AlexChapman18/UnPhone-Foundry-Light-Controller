/**
 * Main file.
 * 
 * Author: Kush Bharakhada and Alex Chapman
 * sketch.ino
*/

#include <fixture_utils.h>
#include <unPhone.h>
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

// Initialise UnPhone
unPhone nuphone = unPhone();

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
    long yMin = 420, yMax = 3915;

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

// TEMPORARY EVENT HANDLER; NEEDS DEFINING
static void evtHandlerArchitecturalGroup(lv_event_t * e) {
    lv_event_code_t code = lv_event_get_code(e);
    if(code == LV_EVENT_CLICKED) {
      Serial.println("Click!");
    }
}

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ SCREENS (PAGES) ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

void renderArchitecturalScreen() {
    // Initialise the architectural screen
    architectural_screen = lv_obj_create(NULL);

    // Define width, height and style for all buttons on the page
    static lv_style_t button_style;
    lv_color_t button_bg_color = LV_COLOR_MAKE(0, 0, 0);
    const int BUTTON_WIDTH = 90, BUTTON_HEIGHT = 75;

    // Design the layout of the architectural screen
    createLabel(70, 20, "Select Architectural Group", architectural_screen);
    createButton(evtHandlerArchitecturalGroup, 20,  50, BUTTON_WIDTH, BUTTON_HEIGHT, "Toilets",        
                 button_bg_color, LV_COLOR_MAKE(255, 255, 255), &button_style, architectural_screen);
    createButton(evtHandlerArchitecturalGroup, 115, 50, BUTTON_WIDTH, BUTTON_HEIGHT, "Main\nExit",
                 button_bg_color, LV_COLOR_MAKE(255, 255, 255), &button_style, architectural_screen);
    createButton(evtHandlerArchitecturalGroup, 210, 50, BUTTON_WIDTH, BUTTON_HEIGHT, "Merch\nLights",
                 button_bg_color, LV_COLOR_MAKE(255, 255, 255), &button_style, architectural_screen);
    createButton(evtHandlerArchitecturalGroup, 20,  130, BUTTON_WIDTH, BUTTON_HEIGHT, "Pillars",
                 button_bg_color, LV_COLOR_MAKE(255, 255, 255), &button_style, architectural_screen);
    createButton(evtHandlerArchitecturalGroup, 115, 130, BUTTON_WIDTH, BUTTON_HEIGHT, "Dancefloor",
                 button_bg_color, LV_COLOR_MAKE(255, 255, 255), &button_style, architectural_screen);
    createButton(evtHandlerArchitecturalGroup, 210, 130, BUTTON_WIDTH, BUTTON_HEIGHT, "Main Bar\nLeft",
                 button_bg_color, LV_COLOR_MAKE(255, 255, 255), &button_style, architectural_screen);
    createButton(evtHandlerArchitecturalGroup, 20,  210, BUTTON_WIDTH, BUTTON_HEIGHT, "Main Bar\nRight",
                 button_bg_color, LV_COLOR_MAKE(255, 255, 255), &button_style, architectural_screen);
    createButton(evtHandlerArchitecturalGroup, 115, 210, BUTTON_WIDTH, BUTTON_HEIGHT, "Bar 1",
                 button_bg_color, LV_COLOR_MAKE(255, 255, 255), &button_style, architectural_screen);
    createButton(evtHandlerArchitecturalGroup, 210, 210, BUTTON_WIDTH, BUTTON_HEIGHT, "Bar 2",
                 button_bg_color, LV_COLOR_MAKE(255, 255, 255), &button_style, architectural_screen);
    createButton(evtHandlerArchitecturalGroup, 20,  290, BUTTON_WIDTH, BUTTON_HEIGHT, "Bar 3",
                 button_bg_color, LV_COLOR_MAKE(255, 255, 255), &button_style, architectural_screen);
    createButton(evtHandlerArchitecturalGroup, 115, 290, BUTTON_WIDTH, BUTTON_HEIGHT, "Raised\nArea Back",
                 button_bg_color, LV_COLOR_MAKE(255, 255, 255), &button_style, architectural_screen);
    createButton(evtHandlerArchitecturalGroup, 210, 290, BUTTON_WIDTH, BUTTON_HEIGHT, "Raised\nBar",
                 button_bg_color, LV_COLOR_MAKE(255, 255, 255), &button_style, architectural_screen);
    createButton(evtHandlerArchitecturalGroup, 20,  370, BUTTON_WIDTH, BUTTON_HEIGHT, "Raised\nFOH",
                 button_bg_color, LV_COLOR_MAKE(255, 255, 255), &button_style, architectural_screen);
    createButton(evtHandlerArchitecturalGroup, 115, 370, BUTTON_WIDTH, BUTTON_HEIGHT, "All\nArcs",
                 button_bg_color, LV_COLOR_MAKE(255, 255, 255), &button_style, architectural_screen);
    createButton(evtHandlerArchitecturalGroup, 210, 370, BUTTON_WIDTH, BUTTON_HEIGHT, "All\nBars",
                 button_bg_color, LV_COLOR_MAKE(255, 255, 255), &button_style, architectural_screen);
}

void switchToArchitecturalScreen() {
    renderArchitecturalScreen();
    lv_scr_load(architectural_screen);
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
  nuphone.backlight(true);

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

  // Render and load the initial screen
  renderArchitecturalScreen();
  lv_scr_load(architectural_screen);
}

void loop() { lv_timer_handler(); }