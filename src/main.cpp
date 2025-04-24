#include <Arduino.h>
#include <WiFi.h>
#include "wifi_settings.h"

#include <lvgl.h>
#include <TFT_eSPI.h>
#include "touch_display.h"
#include "ui/ui.h"
#include "setup_ntp_time.h"

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

    wifi_setup();  // Inicializiramo WiFi povezavo - klic funkcije iz wifi_settings.h
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
    fix_wifi_ui_textarea(); // fix nastavitve ui
    setup_ntp_time();       // kliči setup_ntp_time() iz setup_ntp_time.h

    // Vse te nastavitve spodaj lahko nastavite že v EEZ Studio, če uporabite "flow" način
    //  Nastavimo oznake - labele na simbole
    // lv_label_set_text(objects.wi_fi_bli, LV_SYMBOL_WIFI); // v eez studio nastavimo na simbol wifi tako, da v polje vnesemo  \uf1eb (unicode kodo simbola LV_SYMBOL_WIFI)
    lv_label_set_text(objects.b1_label, LV_SYMBOL_SETTINGS); // za demo sta prikazana oba načina

    // še postavimo wifi na rdečo barvo
    lv_obj_set_style_text_color(objects.wi_fi_bli, lv_color_hex(0xFF0000), 0); // nastavimo barvo besedila na rdečo

    // Nastavimo event pritiska na B1 in sicer na menjavo zaslona nastavitve
    lv_obj_add_event_cb(objects.b1, [](lv_event_t *event) {
        // Pritisnjen je bil gumb B1, naredimo menjavo zaslona
        lv_scr_load(objects.nastavitve); }, LV_EVENT_CLICKED, NULL);

    lv_obj_add_event_cb(objects.b_back, [](lv_event_t *event) {
        // Pritisnjen je bil gumb BACK, naredimo menjavo zaslona
        lv_screen_load(objects.main); }, LV_EVENT_CLICKED, NULL);

    // tipkovnico na drugem zaslonu nastavimo, da vnaša v polje za SSID
    lv_keyboard_set_textarea(objects.kbd, objects.tb_ssid_text);

    // klik na polje za vnos prestavi vnos s tipkovnice na to polje - dodamo povratni klic - ker je kratek, kar anonimno lambda funkcijo
    lv_obj_add_event_cb(objects.tb_ssid_text, [](lv_event_t *event) { lv_keyboard_set_textarea(objects.kbd, objects.tb_ssid_text); }, LV_EVENT_CLICKED, NULL);
    lv_obj_add_event_cb(objects.tb_ssid_pass, [](lv_event_t *event) { lv_keyboard_set_textarea(objects.kbd, objects.tb_ssid_pass); }, LV_EVENT_CLICKED, NULL);

    // gumb b_connect povežemo z funkcijo, ki bo poskusila povezati na SSID
    lv_obj_add_event_cb(objects.b_connect, [](lv_event_t *event) {
        // Pritisnjen je bil gumb b_connect na ekranu nastavitve, naredimo menjavo zaslona
        Serial.println("Pritisnjen gumb connect");
        // Pridobimo vsebino polja za SSID
        const char * ssid = lv_textarea_get_text(objects.tb_ssid_text);
        const char * pass = lv_textarea_get_text(objects.tb_ssid_pass);

        Serial.printf("Povezujem se na SSID: %s z geslom: %s\n", ssid, pass);
        WiFi.begin(ssid, pass); }, LV_EVENT_CLICKED, NULL);

    // tu pride še shranjevanje nastavitev v Preferences
}

tm tm_info;
int prev_sec = 0;
bool wifi_flag_visible = false;

void loop() {
    // blink wifi icon, if wifi not connected
    bool new_wifi_flag_visible;
    if (!WiFi.isConnected()) {
        new_wifi_flag_visible = (millis() % 1000 < 500);
    } else if (WiFi.isConnected()) {
        wifi_flag_visible = true;
        new_wifi_flag_visible = true;
        lv_obj_remove_flag(objects.wi_fi_bli, LV_OBJ_FLAG_HIDDEN);
        // make it green
        lv_obj_set_style_text_color(objects.wi_fi_bli, lv_color_hex(0x00FF00), 0); // nastavimo barvo besedila na zeleno
    }
    if (wifi_flag_visible != new_wifi_flag_visible) {
        wifi_flag_visible = new_wifi_flag_visible;
        if (wifi_flag_visible) {
            lv_obj_add_flag(objects.wi_fi_bli, LV_OBJ_FLAG_HIDDEN);
#ifdef LED_R
            // utripamo z rdečo diodo, ampak tako močno
            analogWrite(LED_R, 255); // turn on LED
#endif

        } else {
            lv_obj_remove_flag(objects.wi_fi_bli, LV_OBJ_FLAG_HIDDEN);
#ifdef LED_R
            // utripamo z rdečo diodo, ampak tako močno
            analogWrite(LED_R, 192); // turn on LED
#endif
        }
    }

    // get current time as tm struct
    getLocalTime(&tm_info);
    if (prev_sec != tm_info.tm_sec) {
        // calculate degrees for each hand
        double h_deg = (tm_info.tm_hour % 12) * 30 + tm_info.tm_min * 0.5;
        double m_deg = tm_info.tm_min * 6 + tm_info.tm_sec * 0.1;
        double s_deg = tm_info.tm_sec * 6;
        // set rotation of hour hand using current hour and minute
        lv_obj_set_style_transform_rotation(objects.img_h_hand, (int)(h_deg * 10), 0);

        // set rotation of minute hand using current minute and second
        lv_obj_set_style_transform_rotation(objects.img_m_hand, (int)(m_deg * 10), 0);

        // set rotation of second hand using current second
        lv_obj_set_style_transform_rotation(objects.img_s_hand, (int)(s_deg * 10), 0);

        prev_sec = tm_info.tm_sec;
    }

    lv_tick_inc(millis() - lastTick); // Update the tick timer. Tick is new for LVGL 9
    lastTick = millis();
    lv_timer_handler(); // Update the UI
    delay(5);
}