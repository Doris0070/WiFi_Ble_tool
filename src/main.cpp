#include <Arduino.h>
#include <WiFi.h>
#include "wifi_settings.h"

#include <lvgl.h>
#include <TFT_eSPI.h>
#include "touch_display.h"
#include "ui/ui.h"
#include "setup_ntp_time.h"
#include "wifi_page_functions.h"
#include "ble_page_functions.h"

/* Get screen resolution from platformio.ini */
// #define TFT_HOR_RES 240 
// #define TFT_VER_RES 320

/*LVGL draw into this buffer, 1/10 screen size usually works well. The size is in bytes*/
#define DRAW_BUF_SIZE (TFT_HOR_RES * TFT_VER_RES / 10 * (LV_COLOR_DEPTH / 8))

#if LV_USE_LOG != 0
void my_print(lv_log_level_t level, const char *buf) {
    LV_UNUSED(level);
    Serial.println(buf);
    Serial.flush();
}
#endif

/* LVGL calls it when a rendered image needs to copied to the display*/
void my_disp_flush(lv_display_t *disp, const lv_area_t *area, uint8_t *px_map) {
    lv_disp_flush_ready(disp);
}

lv_indev_t *indev;     // Touchscreen input device
uint8_t *draw_buf;     // draw_buf is allocated on heap otherwise the static area is too big on ESP32 at compile
uint32_t lastTick = 0; // Used to track the tick timer

void setup() {
    Serial.begin(115200);

    // če imamo rdečo led, jo nastavimo na izhod
#ifdef LED_R
    pinMode(LED_R, OUTPUT);
    digitalWrite(LED_R, HIGH); // turn off LED - LED is active when LOW
#endif
    Serial.printf("LVGL demo V%d.%d.%d\n", lv_version_major(), lv_version_minor(), lv_version_patch());

    //wifi_setup();  // Inicializiramo WiFi povezavo - klic funkcije iz wifi_settings.h
    touch_setup(); // Inicializiramo dotik - klic funkcije iz touch_display.h

    // Initialize LVGL - this must be done before any LVGL function calls
    lv_init();
    lv_display_t *disp;
    draw_buf = new uint8_t[DRAW_BUF_SIZE];
    disp = lv_tft_espi_create(TFT_HOR_RES, TFT_VER_RES, draw_buf, DRAW_BUF_SIZE);

    // Initialize the XPT2046 input device driver
    indev = lv_indev_create();
    lv_indev_set_type(indev, LV_INDEV_TYPE_POINTER);
    lv_indev_set_read_cb(indev, my_touchpad_read);
    lv_display_set_rotation(disp, LV_DISP_ROTATION_270); // Use landscape mode
    Serial.println("Setup done");

    ui_init();              // inicializiramo uporabniški vmesnik, narejen z EEZ Studio
    //fix_wifi_ui_textarea(); // fix nastavitve ui
    //setup_ntp_time();       // kliči setup_ntp_time() iz setup_ntp_time.h

    // Vse te nastavitve spodaj lahko nastavite že v EEZ Studio, če uporabite "flow" način
    //  Nastavimo oznake - labele na simbole
    lv_obj_add_event_cb(objects.ble_button, [](lv_event_t *event) {
        // Pritisnjen je bil gumb BACK, naredimo menjavo zaslona
        lv_screen_load(objects.ble_settings); }, LV_EVENT_CLICKED, NULL);
    lv_obj_add_event_cb(objects.wifi_button, [](lv_event_t *event) {
        // Pritisnjen je bil gumb BACK, naredimo menjavo zaslona
        lv_screen_load(objects.wifi_settings); }, LV_EVENT_CLICKED, NULL);
    lv_obj_add_event_cb(objects.bluetooth_page_back_button, [](lv_event_t *event) {
        // Pritisnjen je bil gumb BACK, naredimo menjavo zaslona
        lv_screen_load(objects.main); }, LV_EVENT_CLICKED, NULL);
    lv_obj_add_event_cb(objects.wifi_page_back_button_1, [](lv_event_t *event) {
        // Pritisnjen je bil gumb BACK, naredimo menjavo zaslona
        lv_screen_load(objects.main); }, LV_EVENT_CLICKED, NULL);
}

void loop() {
    //Pust notr kr je nujno za refreshat zaslon!!!!!!!!!!!!!!!!!!!!!!
    lv_tick_inc(millis() - lastTick); // Update the tick timer. Tick is new for LVGL 9
    lastTick = millis();
    lv_timer_handler(); // Update the UI
    delay(5);
}