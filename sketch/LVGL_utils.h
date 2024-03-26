/**
 * LVGL utils header file.
 * 
 * Author: Kush Bharakhada and Alex Chapman
 * LVGL_utils.h
*/

#ifndef LVGL_UTILS_H
#define LVGL_UTILS_H

#include <lvgl.h>

void create_button(lv_event_cb_t event_handler, int position_x, int position_y,
                   int size_x, int size_y, char *button_text,  lv_color_t bg_color,
                   lv_color_t text_color, lv_style_t *style_pointer, lv_obj_t *screen);


void create_slider(lv_event_cb_t event_handler, int position_x, int position_y,
                   int size_x, int size_y, double proportion, lv_color_t bg_color,
                   lv_style_t *style_pointer, lv_obj_t *screen);


void create_label(int position_x, int position_y, char *text, lv_obj_t *screen);

#endif
