/**
 * LVGL utils header file.
 * 
 * Author: Kush Bharakhada and Alex Chapman
 * LVGL_utils.h
*/

#ifndef LVGL_UTILS_H
#define LVGL_UTILS_H

#include <lvgl.h>

void createButton(lv_event_cb_t event_handler, int position_x, int position_y, int size_x, int size_y,
                  const char *button_text,  lv_color_t bg_color, lv_color_t text_color, lv_coord_t rounded,
                  lv_style_t *style_pointer, lv_obj_t *screen);


void createSlider(lv_event_cb_t event_handler, int position_x, int position_y,
                  int size_x, int size_y, float proportion, lv_color_t bg_color,
                  lv_style_t *style_pointer, lv_obj_t *screen);


void createLabel(int position_x, int position_y, const char *text, lv_obj_t *screen);

#endif
