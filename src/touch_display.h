/*Read the touchpad*/
#pragma once
#include <lvgl.h>
#include "c_touch/CST820.h"

#ifdef RESISTIVE_TOUCH
// Touch Screen pins
// ----------------------------
// The CYD touch uses some non default SPI pins
#define XPT2046_IRQ 36
#define XPT2046_MOSI 32
#define XPT2046_MISO 39
#define XPT2046_CLK 25
#define XPT2046_CS 33
#include <XPT2046_Touchscreen.h>
SPIClass touchscreenSpi = SPIClass(VSPI);
XPT2046_Touchscreen touchscreen(XPT2046_CS, XPT2046_IRQ);
uint16_t touchScreenMinimumX = 200, touchScreenMaximumX = 3700, touchScreenMinimumY = 240, touchScreenMaximumY = 3800;
#endif
#ifdef CAPACITIVE_TOUCH
// We use I2C for the capacitive touch
#define I2C_SDA 33
#define I2C_SCL 32
#define TP_RST 25
#define TP_INT 21
#endif

#ifdef RESISTIVE_TOUCH
void my_touchpad_read(lv_indev_t *indev, lv_indev_data_t *data)
{
    if (touchscreen.touched())
    {
        TS_Point p = touchscreen.getPoint();
        // Some very basic auto calibration so it doesn't go out of range
        if (p.x < touchScreenMinimumX)
            touchScreenMinimumX = p.x;
        if (p.x > touchScreenMaximumX)
            touchScreenMaximumX = p.x;
        if (p.y < touchScreenMinimumY)
            touchScreenMinimumY = p.y;
        if (p.y > touchScreenMaximumY)
            touchScreenMaximumY = p.y;
        // Map this to the pixel position
        data->point.x = map(p.x, touchScreenMinimumX, touchScreenMaximumX, 1, TFT_HOR_RES); /* Touchscreen X calibration */
        data->point.y = map(p.y, touchScreenMinimumY, touchScreenMaximumY, 1, TFT_VER_RES); /* Touchscreen Y calibration */
        data->state = LV_INDEV_STATE_PRESSED;
        /*
        Serial.print("Touch x ");
        Serial.print(data->point.x);
        Serial.print(" y ");
        Serial.println(data->point.y);
        */
        return;
    }
}
#endif

#ifdef CAPACITIVE_TOUCH

CST820 touch(I2C_SDA, I2C_SCL, TP_RST, TP_INT);

void my_touchpad_read(lv_indev_t *indev, lv_indev_data_t *data)
{

    bool touched;
    uint8_t gesture;
    uint16_t touchX, touchY;

    touched = touch.getTouch(&touchX, &touchY, &gesture);
    touchX = 240 - touchX;
    touchY = 320 - touchY;

    if (!touched)
    {
        data->state = LV_INDEV_STATE_REL;
    }
    else
    {
        data->state = LV_INDEV_STATE_PR;
        /*Set the coordinates*/
        data->point.x = touchX;
        data->point.y = touchY;
    }
    // Serial.print("Touch x ");
    // Serial.print(data->point.x);
    // Serial.print(" y ");
    // Serial.println(data->point.y);
    return;
    #endif
    
    // if no touch defined, set state to released
    data->state = LV_INDEV_STATE_RELEASED;
}


void touch_setup()
{
    // Initialise the touchscreen
#ifdef RESISTIVE_TOUCH
    touchscreenSpi.begin(XPT2046_CLK, XPT2046_MISO, XPT2046_MOSI, XPT2046_CS); /* Start second SPI bus for touchscreen */
    touchscreen.begin(touchscreenSpi);                                         /* Touchscreen init */
    touchscreen.setRotation(2);                                                /* Inverted landscape orientation to match screen */
#endif
#ifdef CAPACITIVE_TOUCH
    touch.begin();
#endif
}