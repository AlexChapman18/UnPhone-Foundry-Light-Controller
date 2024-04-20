/**
 * LVGL utils implementation. Contains implementations to simplify the process
 * of creating buttons, sliders, and labels.
 * 
 * Author: Kush Bharakhada and Alex Chapman (2024)
 * Filename: LVGL_utils.cpp
*/

#include <LVGL_utils.h>

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ CREATING BUTTONS ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

void createButton(lv_event_cb_t event_handler, int position_x, int position_y, int width, int height,
                  const char *button_text, lv_color_t bg_color, lv_color_t text_color, lv_coord_t rounded,
                  lv_style_t *style_pointer, lv_obj_t *screen) {
    
    // Create button
    lv_obj_t *button = lv_btn_create(screen);
    lv_obj_add_event_cb(button, event_handler, LV_EVENT_ALL, NULL);

    // Style the button
    lv_style_init(style_pointer);
    lv_style_set_bg_color(style_pointer, bg_color);
    lv_style_set_text_color(style_pointer, text_color);
    lv_style_set_border_width(style_pointer, 2);
    lv_style_set_radius(style_pointer, rounded);
    lv_style_set_border_color(style_pointer, lv_color_black());
    lv_obj_add_style(button, style_pointer, 0);

    // Set size and position of button
    lv_obj_set_size(button, width, height);
    lv_obj_set_pos(button, position_x, position_y);

    // Add label to button
    lv_obj_t *label = lv_label_create(button);
    lv_obj_set_style_text_align(label, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN);
    lv_label_set_text(label, button_text);
    lv_obj_center(label);
}

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ CREATING RECTANGLE ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

lv_obj_t *createRectangle(int position_x, int position_y, int width, int height, lv_color_t bg_color,
                          lv_coord_t rounded, lv_style_t *style_pointer, lv_obj_t *screen) {
    
    // Create rectangle
    lv_obj_t *rectangle = lv_obj_create(screen);

    // Style the rectangle
    lv_style_init(style_pointer);
    lv_style_set_bg_color(style_pointer, bg_color);
    lv_style_set_border_width(style_pointer, 2);
    lv_style_set_radius(style_pointer, rounded);
    lv_style_set_border_color(style_pointer, lv_color_black());
    lv_obj_add_style(rectangle, style_pointer, 0);

    // Set size and position of rectangle
    lv_obj_set_size(rectangle, width, height);
    lv_obj_set_pos(rectangle, position_x, position_y);

    return rectangle;
}

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ CREATE SLIDER ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

lv_obj_t *createSlider(lv_event_cb_t event_handler, int position_x, int position_y, int width, int height,
                       float proportion, lv_color_t bg_color, lv_style_t *style_pointer, lv_obj_t *screen) {

	  // Create a slider				
    lv_obj_t *slider = lv_slider_create(screen);
    lv_obj_add_event_cb(slider, event_handler, LV_EVENT_VALUE_CHANGED, NULL);

    // Set styling on the slider
    lv_style_init(style_pointer);
    lv_obj_set_style_bg_color(slider, LV_COLOR_MAKE(100, 100, 100), LV_PART_KNOB);
    lv_obj_set_style_bg_opa(slider, LV_OPA_100, LV_PART_KNOB);
    lv_obj_set_style_bg_color(slider, lv_color_black(), LV_PART_MAIN);
    lv_obj_set_style_bg_opa(slider, LV_OPA_100, LV_PART_MAIN);
    lv_obj_set_style_bg_color(slider, bg_color, LV_PART_INDICATOR);
    lv_obj_set_style_bg_opa(slider, LV_OPA_100, LV_PART_INDICATOR);
    lv_obj_add_style(slider, style_pointer, 0);

    // Set size and position of slider
    lv_obj_set_size(slider, width, height);
    lv_obj_set_pos(slider, position_x, position_y);

    // Slider range is from 0-255 as most sliders will use this
    lv_slider_set_range(slider, 0, 255);

    // Set slider value
    lv_slider_set_value(slider, (uint8_t)proportion, LV_ANIM_ON);
    return slider;
}

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ CREATE LABEL ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

lv_obj_t *createLabel(int position_x, int position_y, const char *text, lv_obj_t *screen) {
    // Create and position label
    lv_obj_t *label = lv_label_create(screen);
    lv_label_set_text(label, text);
    lv_obj_set_style_text_align(label, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN);
    lv_obj_set_pos(label, position_x, position_y);

    return label;
}
