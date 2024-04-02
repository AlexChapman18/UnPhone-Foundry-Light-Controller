/**
 * LVGL utils header file.
 * Contains function headers to simplify the process of creating
 * buttons, sliders, and labels.
 * 
 * Author: Kush Bharakhada and Alex Chapman
 * LVGL_utils.h
*/

#ifndef LVGL_UTILS_H
#define LVGL_UTILS_H

#include <lvgl.h>

/**
 * Designs and creates an LVGL button.
 *
 * @param event_handler   Event handler to be attached to the button created.
 * @param position_x      Button position on the x-axis.
 * @param position_y      Button position on the y-axis.
 * @param width           Button width.
 * @param height          Button height.
 * @param button_text     Text on the button.
 * @param bg_color        Button background color.
 * @param text_color      Button text color.
 * @param rounded         Roundness of the button corners.
 * @param style_pointer   Style object to store the style (not created in this function otherwise
 *                        styles will not render correctly when used in main).
 * @param screen          Screen to attach the button object to.
 */
void createButton(lv_event_cb_t event_handler, int position_x, int position_y, int width, int height,
                  const char *button_text,  lv_color_t bg_color, lv_color_t text_color, lv_coord_t rounded,
                  lv_style_t *style_pointer, lv_obj_t *screen);


/**
 * Designs and creates an LVGL slider.
 *
 * @param event_handler   Event handler to be attached to the slider created.
 * @param position_x      Slider position on the x-axis.
 * @param position_y      Slider position on the y-axis.
 * @param width           Slider width.
 * @param height          Slider height.
 * @param proportion      Default position to render the slider knob at.
 * 

 * @param bg_color        Slider background color.
 * @param style_pointer   Style object to store the style (not created in this function otherwise
 *                        styles will not render correctly when used in main).
 * @param screen          Screen to attach the slider object to.
 * @param slider          Slider object, to allow slider methods to be used in main on the slider.
 */
lv_obj_t *createSlider(lv_event_cb_t event_handler, int position_x, int position_y, int width, int height,
                        float proportion, lv_color_t bg_color, lv_style_t *style_pointer, lv_obj_t *screen);


/**
 * Creates an LVGL label for text.
 *
 * @param position_x      Label position on the x-axis.
 * @param position_y      Label position on the y-axis.
 * @param text            Text to add onto the label.
 * @param screen          Screen to attach the text object to.
 */
void createLabel(int position_x, int position_y, const char *text, lv_obj_t *screen);

#endif
