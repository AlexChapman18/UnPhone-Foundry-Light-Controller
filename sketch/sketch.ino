/**
 * Main file.
 * 
 * Author: Kush Bharakhada and Alex Chapman
 * sketch.ino
*/

#include <fixture_utils.h>
#include <nuPhone.h>
#include <TFT_eSPI.h>
#include <LVGL_utils.h>

static const uint16_t screenWidth  = 320;
static const uint16_t screenHeight = 480;
TFT_eSPI tft = TFT_eSPI(screenWidth, screenHeight);
static lv_disp_draw_buf_t draw_buf;
static lv_color_t buf[ screenWidth * 10 ];

nuPhone nuphone = nuPhone();

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