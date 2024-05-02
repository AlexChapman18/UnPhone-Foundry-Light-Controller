# 1 "C:\\Users\\agent\\AppData\\Local\\Temp\\tmpmpetkiyv"
#include <Arduino.h>
# 1 "C:/Users/agent/Documents/Git_Repositories/UnPhone-Foundry-Light-Controller/sketch/sketch.ino"







#include <wifi_utils.h>
#include <artnet_utils.h>
#include <architecture_utils.h>
#include <NuPhone.h>
#include <TFT_eSPI.h>
#include <LVGL_utils.h>
#include <Adafruit_SPIFlash.h>
#include <private.h>





static const uint16_t screenWidth = 320;
static const uint16_t screenHeight = 480;
TFT_eSPI tft = TFT_eSPI(screenWidth, screenHeight);
static lv_disp_draw_buf_t draw_buf;
static lv_color_t buf[screenWidth * 10];
boolean is_init_boot = true;


NuPhone nuphone = NuPhone();

ESPWiFi espwifi = ESPWiFi();

ArtNetUniverse anu = ArtNetUniverse();

ArchitectureGroup architecture_group_list[15];

ArchitectureGroup *current_arc_group;


unsigned long last_bar_update = 0;


static lv_style_t signal_bar1_style, signal_bar2_style, signal_bar3_style, error_bar_style;
lv_obj_t *signal_bar1;
lv_obj_t *signal_bar2;
lv_obj_t *signal_bar3;
lv_obj_t *error_bar;


static lv_obj_t *architecture_screen;
static lv_obj_t *color_screen;
static lv_obj_t *intensity_effects_screen;
static lv_obj_t *color_status_screen;
static int current_screen;


static lv_obj_t *red_slider;
static lv_obj_t *green_slider;
static lv_obj_t *blue_slider;





const char* colors_list[] = {"orange", "red", "rose", "magenta", "violet", "blue", "azure", "cyan",
                             "aquamarine", "green", "chartreuse", "yellow", "White", "Off"};

const int color_values_list[][3] = {{255, 127, 0}, {255, 0, 0}, {255, 0, 127}, {255, 0, 255}, {127, 0, 255},
                                    {0, 0, 255}, {0, 127, 255}, {0, 255, 255}, {0, 255, 127}, {0, 255, 0},
                                    {127, 255, 0}, {255, 255, 0}, {255, 255, 255}, {0, 0, 0}};


const char* architectures_list[] = {"Toilets", "Main\nExit", "Merch\nLights", "Pillars", "Dancefloor", "Main Bar\nLeft",
                                    "Main Bar\nRight", "Bar 1", "Bar 2", "Bar 3", "Raised\nArea Back", "Raised\nBar",
                                    "Raised\nFOH", "All\nArcs", "All\nBars"};

const char* effects_list[] = {"Solid", "Pulse\nEffect", "Odd-Even\nEffect", "Fade\nSwipe", "Binary\nSwipe", "Bars\nFade"};
# 86 "C:/Users/agent/Documents/Git_Repositories/UnPhone-Foundry-Light-Controller/sketch/sketch.ino"
long myMapper(long x, long in_min, long in_max, long out_min, long out_max);
void touchpadRead(lv_indev_drv_t *indev_driver, lv_indev_data_t *data);
void displayFlush(lv_disp_drv_t *disp, const lv_area_t *area, lv_color_t *color_p);
static void evtHandlerColorBtns(lv_event_t *e);
static void evtHandlerArchGroupBtns(lv_event_t *e);
static void evtHandlerEffectsBtns(lv_event_t * e);
static void evtHandlerBackBtn(lv_event_t *e);
static void evtHandlerRedSlider(lv_event_t *e);
static void evtHandlerGreenSlider(lv_event_t *e);
static void evtHandlerBlueSlider(lv_event_t *e);
static void evtHandlerIntensitySlider(lv_event_t *e);
static void evtHandlerSpeedSlider(lv_event_t *e);
void renderArchitectureScreen();
void renderColorScreen(ArchitectureGroup *current_group);
void renderIntensityEffectsScreen();
void renderColorStatusScreen();
void deletePreviousScreen();
void initialiseSignalStrengthBars(lv_obj_t *screen);
void drawSignalStrength(uint8_t wifi_status);
void setup();
void loop();
#line 86 "C:/Users/agent/Documents/Git_Repositories/UnPhone-Foundry-Light-Controller/sketch/sketch.ino"
long myMapper(long x, long in_min, long in_max, long out_min, long out_max) {
    long probable = (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
    if(probable < out_min) return out_min;
    if(probable > out_max) return out_max;
    return probable;
}




void touchpadRead(lv_indev_drv_t *indev_driver, lv_indev_data_t *data) {
    uint16_t touchX, touchY;
    bool touched = nuphone.tsp->touched();

    if (!touched) {
        data->state = LV_INDEV_STATE_REL;
    }
    else {
        data->state = LV_INDEV_STATE_PR;


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





void displayFlush(lv_disp_drv_t *disp, const lv_area_t *area, lv_color_t *color_p) {
    uint32_t w = (area->x2 - area->x1 + 1);
    uint32_t h = (area->y2 - area->y1 + 1);
    tft.startWrite();
    tft.setAddrWindow(area->x1, area->y1, w, h);
    tft.pushColors((uint16_t*)&color_p->full, w * h, true);
    tft.endWrite();
    lv_disp_flush_ready(disp);
}
# 147 "C:/Users/agent/Documents/Git_Repositories/UnPhone-Foundry-Light-Controller/sketch/sketch.ino"
static void evtHandlerColorBtns(lv_event_t *e) {
    lv_event_code_t code = lv_event_get_code(e);
    if(code == LV_EVENT_CLICKED) {

        const char *text = lv_label_get_text(lv_obj_get_child(lv_event_get_target(e), NULL));

        for (int i=0; i < sizeof(colors_list)/sizeof(colors_list[0]); i++) {
            if (strcmp(text, colors_list[i]) == 0) {
                current_arc_group->setRGB(color_values_list[i][0], color_values_list[i][1], color_values_list[i][2]);

                lv_slider_set_value(red_slider, color_values_list[i][0], LV_ANIM_ON);
                lv_slider_set_value(green_slider, color_values_list[i][1], LV_ANIM_ON);
                lv_slider_set_value(blue_slider, color_values_list[i][2], LV_ANIM_ON);
                break;
            }
        }
    }
}






static void evtHandlerArchGroupBtns(lv_event_t *e) {
    lv_event_code_t code = lv_event_get_code(e);
    if(code == LV_EVENT_CLICKED) {

        const char *text = lv_label_get_text(lv_obj_get_child(lv_event_get_target(e), NULL));

        for (int i=0; i < sizeof(architectures_list)/sizeof(architectures_list[0]); i++) {
            if (strcmp(text, architectures_list[i]) == 0) {
                renderColorScreen(&architecture_group_list[i]);
                lv_scr_load(color_screen);
                deletePreviousScreen();
                current_screen = 1;
                break;
            }
        }
    }
}






static void evtHandlerEffectsBtns(lv_event_t * e) {
    lv_event_code_t code = lv_event_get_code(e);
    if(code == LV_EVENT_CLICKED) {

        const char *text = lv_label_get_text(lv_obj_get_child(lv_event_get_target(e), NULL));

        for (int i=0; i < sizeof(effects_list)/sizeof(effects_list[0]); i++) {
            if (strcmp(text, effects_list[i]) == 0) {
                anu.current_effect = i;
                break;
            }
        }
    }
}






static void evtHandlerBackBtn(lv_event_t *e) {
    lv_event_code_t code = lv_event_get_code(e);
    if(code == LV_EVENT_CLICKED) {
        renderArchitectureScreen();
        lv_scr_load(architecture_screen);
        deletePreviousScreen();
        current_screen = 0;
    }
}






static void evtHandlerRedSlider(lv_event_t *e) {
    lv_obj_t *slider = lv_event_get_target(e);
    uint8_t value = (uint8_t)lv_slider_get_value(slider);
    current_arc_group->setRed(value);
}






static void evtHandlerGreenSlider(lv_event_t *e) {
    lv_obj_t *slider = lv_event_get_target(e);
    uint8_t value = (uint8_t)lv_slider_get_value(slider);
    current_arc_group->setGreen(value);
}






static void evtHandlerBlueSlider(lv_event_t *e) {
    lv_obj_t *slider = lv_event_get_target(e);
    uint8_t value = (uint8_t)lv_slider_get_value(slider);
    current_arc_group->setBlue(value);
}






static void evtHandlerIntensitySlider(lv_event_t *e) {
    lv_obj_t *slider = lv_event_get_target(e);
    uint8_t value = (uint8_t)lv_slider_get_value(slider);

    float normalised_value = (float)value / 255.0;
    anu.setIntensity(normalised_value);
}






static void evtHandlerSpeedSlider(lv_event_t *e) {
    lv_obj_t *slider = lv_event_get_target(e);
    uint8_t value = (uint8_t)lv_slider_get_value(slider);

    float normalised_value = (float)value / 255.0;
    anu.setSpeed(normalised_value);
}







void renderArchitectureScreen() {
    architecture_screen = lv_obj_create(NULL);


    initialiseSignalStrengthBars(architecture_screen);


    static lv_style_t btn_style;


    const int BUTTON_WIDTH = 90, BUTTON_HEIGHT = 75;
    lv_color_t BTN_BG_COLOR = LV_COLOR_MAKE(35, 35, 35);
    lv_coord_t BTN_ROUNDED = 5;

    int initial_x = 15, initial_y = 50;
    int PADDING_X = 10, PADDING_Y = 5;


    createLabel(70, 13, "Select Architecture Group", architecture_screen);
    for (int i = 0; i < sizeof(architecture_group_list)/sizeof(architecture_group_list[0]); i++) {
        createButton(evtHandlerArchGroupBtns, initial_x, initial_y, BUTTON_WIDTH, BUTTON_HEIGHT, architectures_list[i],
                     BTN_BG_COLOR, lv_color_white(), BTN_ROUNDED, &btn_style, architecture_screen);


        initial_x += BUTTON_WIDTH + PADDING_X;
        if ((i + 1) % 3 == 0) {
            initial_x = 15;
            initial_y += BUTTON_HEIGHT + PADDING_Y;
        }
    }
}






void renderColorScreen(ArchitectureGroup *current_group) {
    current_arc_group = current_group;
    color_screen = lv_obj_create(NULL);


    initialiseSignalStrengthBars(color_screen);


    static lv_style_t back_btn_style, black_btn_style, white_btn_style,
                      red_slider_style, green_slider_style, blue_slider_style;


    static lv_style_t color_styles_list[sizeof(colors_list) / sizeof(colors_list[0])-2];
    for (int i = 0; i < sizeof(colors_list) / sizeof(colors_list[0])-2; i++) {
        lv_style_init(&color_styles_list[i]);
    }


    const int BUTTON_WIDTH = 90, BUTTON_HEIGHT = 30;
    const int WHITE_AND_OFF_BUTTON_WIDTH = 70, WHITE_AND_OFF_BUTTON_HEIGHT = 80;
    const int SLIDER_WIDTH = 20, SLIDER_HEIGHT = 210;
    lv_coord_t COLOR_BTN_ROUNDED = 15;
    lv_coord_t REG_BTN_ROUNDED = 5;

    int initial_x = 20, initial_y = 55;
    int PADDING = 5;


    createLabel(150, 13, "Lighting Color", color_screen);
    createButton(evtHandlerBackBtn, 20, 10, BUTTON_WIDTH, BUTTON_HEIGHT, "Back", lv_color_black(), lv_color_white(),
                 REG_BTN_ROUNDED, &back_btn_style, color_screen);


    for (int i = 0; i < sizeof(colors_list) / sizeof(colors_list[0])-2; i++) {
        lv_color_t color = LV_COLOR_MAKE(color_values_list[i][0], color_values_list[i][1], color_values_list[i][2]);
        createButton(evtHandlerColorBtns, initial_x, initial_y, BUTTON_WIDTH, BUTTON_HEIGHT, colors_list[i],
                     color, color, COLOR_BTN_ROUNDED, &color_styles_list[i], color_screen);


        initial_x += BUTTON_WIDTH + PADDING;
        if ((i + 1) % 3 == 0) {
            initial_x = 20;
            initial_y += BUTTON_HEIGHT + PADDING;
        }
    }


    red_slider = createSlider(evtHandlerRedSlider, 30, 220, SLIDER_WIDTH, SLIDER_HEIGHT, current_arc_group->getRed(),
                              LV_COLOR_MAKE(255, 0, 0), &red_slider_style, color_screen);
    createLabel(22, 450, "Red", color_screen);


    green_slider = createSlider(evtHandlerGreenSlider, 102, 220, SLIDER_WIDTH, SLIDER_HEIGHT, current_arc_group->getGreen(),
                                LV_COLOR_MAKE(0, 200, 0), &green_slider_style, color_screen);
    createLabel(85, 450, "Green", color_screen);


    blue_slider = createSlider(evtHandlerBlueSlider, 180, 220, SLIDER_WIDTH, SLIDER_HEIGHT, current_arc_group->getBlue(),
                               LV_COLOR_MAKE(0, 0, 255), &blue_slider_style, color_screen);
    createLabel(170, 450, "Blue", color_screen);


    createButton(evtHandlerColorBtns, 230, 220, WHITE_AND_OFF_BUTTON_WIDTH, WHITE_AND_OFF_BUTTON_HEIGHT, colors_list[12],
                 lv_color_white(), lv_color_black(), REG_BTN_ROUNDED, &white_btn_style, color_screen);


    createButton(evtHandlerColorBtns, 230, 320, WHITE_AND_OFF_BUTTON_WIDTH, WHITE_AND_OFF_BUTTON_HEIGHT, colors_list[13],
                 lv_color_black(), lv_color_white(), REG_BTN_ROUNDED, &black_btn_style, color_screen);
}




void renderIntensityEffectsScreen() {
    intensity_effects_screen = lv_obj_create(NULL);


    initialiseSignalStrengthBars(intensity_effects_screen);


    static lv_style_t effect_button_style, solid_button_style, back_btn_style, slider_style;


    const int BUTTON_WIDTH = 90, BUTTON_HEIGHT = 55;
    const int SLIDER_WIDTH = 20, SLIDER_HEIGHT = 280;
    lv_coord_t BTN_ROUNDED = 5;
    lv_color_t BTN_BG_COLOR = LV_COLOR_MAKE(45, 45, 45);
    lv_color_t BTN_TEXT_COLOR = LV_COLOR_MAKE(255, 255, 255);

    int initial_y = 70;
    int PADDING = 5;


    createLabel(88, 13, "Intensity and Effects", intensity_effects_screen);
    for (int i = 0; i < sizeof(effects_list) / sizeof(effects_list[0]); i++) {
        createButton(evtHandlerEffectsBtns, 210, initial_y, BUTTON_WIDTH, BUTTON_HEIGHT, effects_list[i],
                     BTN_BG_COLOR, BTN_TEXT_COLOR, BTN_ROUNDED, &solid_button_style, intensity_effects_screen);

        initial_y += BUTTON_HEIGHT + PADDING;
    }


    uint8_t denormalised_intensity_value = (uint8_t)(anu.getIntensity()*255.0);
    uint8_t denormalised_speed_value = (uint8_t)(anu.getSpeed()*255.0);


    createSlider(evtHandlerIntensitySlider, 40, 100, SLIDER_WIDTH, SLIDER_HEIGHT, denormalised_intensity_value, BTN_BG_COLOR,
                 &slider_style, intensity_effects_screen);
    createLabel(20, 405, "Intensity", intensity_effects_screen);


    createSlider(evtHandlerSpeedSlider, 135, 100, SLIDER_WIDTH, SLIDER_HEIGHT, denormalised_speed_value, BTN_BG_COLOR,
                 &slider_style, intensity_effects_screen);
    createLabel(120, 405, "Effect\nSpeed", intensity_effects_screen);
}




void renderColorStatusScreen() {
    color_status_screen = lv_obj_create(NULL);


    initialiseSignalStrengthBars(color_status_screen);


    static lv_style_t styles_list[sizeof(architecture_group_list)/sizeof(architecture_group_list[0])];
    for (int i = 0; i < sizeof(architecture_group_list)/sizeof(architecture_group_list[0]); i++) {
        lv_style_init(&styles_list[i]);
    }


    const int COLOR_BLOCK_WIDTH = 50, COLOR_BLOCK_HEIGHT = 20;
    lv_coord_t COLOR_BLOCK_ROUNDED = 5;
    int initial_x = 30, initial_y = 60;
    int PADDING = 5;


    createLabel(90, 13, "Global Color Status", color_status_screen);
    for (int i=0; i < sizeof(architecture_group_list)/sizeof(architecture_group_list[0]); i++) {

        const char* name = architecture_group_list[i].getName();
        std::string nameString = std::string(name);
        std::replace(nameString.begin(), nameString.end(), '\n', ' ');


        lv_color_t bg_color = LV_COLOR_MAKE(architecture_group_list[i].getRed(),
                                            architecture_group_list[i].getGreen(),
                                            architecture_group_list[i].getBlue());


        createLabel(initial_x, initial_y, nameString.c_str(), color_status_screen);
        createRectangle(initial_x+200, initial_y, COLOR_BLOCK_WIDTH, COLOR_BLOCK_HEIGHT,
                        bg_color, COLOR_BLOCK_ROUNDED, &styles_list[i], color_status_screen);

        initial_y += COLOR_BLOCK_HEIGHT + PADDING;
    }
}




void deletePreviousScreen() {
    if (current_screen == 0) {lv_obj_del(architecture_screen); }
    if (current_screen == 1) {lv_obj_del(color_screen); }
    if (current_screen == 2) {lv_obj_del(intensity_effects_screen); }
    if (current_screen == 3) {lv_obj_del(color_status_screen); }
}
# 504 "C:/Users/agent/Documents/Git_Repositories/UnPhone-Foundry-Light-Controller/sketch/sketch.ino"
void initialiseSignalStrengthBars(lv_obj_t *screen) {
    signal_bar1 = createRectangle(278, 24, 8, 10, lv_color_black(), 0, &signal_bar1_style, screen);
    signal_bar2 = createRectangle(290, 16, 8, 18, lv_color_black(), 0, &signal_bar2_style, screen);
    signal_bar3 = createRectangle(302, 8, 8, 26, lv_color_black(), 0, &signal_bar3_style, screen);
    error_bar = createRectangle(275, 22, 38, 4, LV_COLOR_MAKE(255, 0, 0), 0, &error_bar_style, screen);
    lv_style_set_border_width(&error_bar_style, 0);
    drawSignalStrength(espwifi.getWiFiStrength());
}






void drawSignalStrength(uint8_t wifi_status) {
    if (wifi_status == espwifi.NO_CONNECTION) {
        lv_style_set_bg_opa(&signal_bar1_style, LV_OPA_TRANSP);
        lv_style_set_bg_opa(&signal_bar2_style, LV_OPA_TRANSP);
        lv_style_set_bg_opa(&signal_bar3_style, LV_OPA_TRANSP);
        lv_style_set_bg_opa(&error_bar_style, LV_OPA_90);
    } else if (wifi_status == espwifi.WEAK_CONNECTION) {
        lv_style_set_bg_opa(&signal_bar1_style, LV_OPA_COVER);
        lv_style_set_bg_opa(&signal_bar2_style, LV_OPA_TRANSP);
        lv_style_set_bg_opa(&signal_bar3_style, LV_OPA_TRANSP);
        lv_style_set_bg_opa(&error_bar_style, LV_OPA_TRANSP);
    } else if (wifi_status == espwifi.MODERATE_CONNECTION) {
        lv_style_set_bg_opa(&signal_bar1_style, LV_OPA_COVER);
        lv_style_set_bg_opa(&signal_bar2_style, LV_OPA_COVER);
        lv_style_set_bg_opa(&signal_bar3_style, LV_OPA_TRANSP);
        lv_style_set_bg_opa(&error_bar_style, LV_OPA_TRANSP);
    } else if (wifi_status == espwifi.STRONG_CONNECTION) {
        lv_style_set_bg_opa(&signal_bar1_style, LV_OPA_COVER);
        lv_style_set_bg_opa(&signal_bar2_style, LV_OPA_COVER);
        lv_style_set_bg_opa(&signal_bar3_style, LV_OPA_COVER);
        lv_style_set_bg_opa(&error_bar_style, LV_OPA_TRANSP);
    }
    lv_obj_invalidate(signal_bar1);
    lv_obj_invalidate(signal_bar2);
    lv_obj_invalidate(signal_bar3);
    lv_obj_invalidate(error_bar);
}







void setup() {
    Serial.begin(115200);


    nuphone.begin();
    nuphone.tsp->setRotation(2);
    nuphone.setBacklight(true);


    espwifi.begin();


    tft.begin();
    tft.setRotation(0);


    uint16_t calData[5] = { 347, 3549, 419, 3352, 5 };
    tft.setTouch(calData);


    lv_init();
    lv_disp_draw_buf_init( &draw_buf, buf, NULL, screenWidth * 10 );


    static lv_disp_drv_t disp_drv;
    lv_disp_drv_init(&disp_drv);
    disp_drv.hor_res = screenWidth;
    disp_drv.ver_res = screenHeight;
    disp_drv.flush_cb = displayFlush;
    disp_drv.draw_buf = &draw_buf;
    lv_disp_drv_register(&disp_drv);


    static lv_indev_drv_t indev_drv;
    lv_indev_drv_init(&indev_drv);
    indev_drv.type = LV_INDEV_TYPE_POINTER;
    indev_drv.read_cb = touchpadRead;
    lv_indev_drv_register(&indev_drv);


    for (int i=0; i < sizeof(architecture_group_list)/sizeof(architecture_group_list[0]); i++) {
        architecture_group_list[i] = ArchitectureGroup(i, architectures_list[i]);
    }


    renderArchitectureScreen();
    lv_scr_load(architecture_screen);
    current_screen = 0;
}




void loop() {

    if (espwifi.isConnected() && is_init_boot) {
        anu.setup();
        anu.begin();
        is_init_boot = false;
    }

    lv_timer_handler();


    if (nuphone.isButton1()) {
      if (current_screen != 0) {
        renderArchitectureScreen();
        lv_scr_load(architecture_screen);
        deletePreviousScreen();
        current_screen = 0;
      }
    } else if (nuphone.isButton2()) {
      if (current_screen != 2) {
        renderIntensityEffectsScreen();
        lv_scr_load(intensity_effects_screen);
        deletePreviousScreen();
        current_screen = 2;
      }
    } else if (nuphone.isButton3()) {
      if (current_screen != 3) {
        renderColorStatusScreen();
        lv_scr_load(color_status_screen);
        deletePreviousScreen();
        current_screen = 3;
      }
    }


    unsigned long current_time = millis();
    if (current_time - last_bar_update >= 1000) {
        drawSignalStrength(espwifi.getWiFiStrength());
        last_bar_update = current_time;
    }

    delay(5);
}