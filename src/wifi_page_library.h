// wifi_page_library.h
#ifndef WIFI_PAGE_LIBRARY_H
#define WIFI_PAGE_LIBRARY_H

#include <WiFi.h>
#include <esp_wifi.h>
#include <lvgl.h>   // <-- Needed for timer

#define DEFAULT_CHANNEL 1

struct NetworkInfo {
    String ssid;
    uint8_t bssid[6];
    int channel;
};

extern NetworkInfo targetNetwork;
extern const char **wifi_button_texts;
extern String *wifi_networks;
extern int WIFI_BUTTON_COUNT;
extern int wifi_selected_index;

// ===== NEW - Timer handle for continuous attack =====
extern lv_timer_t *wifi_attack_timer;
extern bool is_jamming;
extern bool is_dos;

// =============== FUNCTIONS ===============
void scanNetworks();
void wifi_jamm();
void wifi_dos();
void wifi_jamm_stop();

#endif // WIFI_PAGE_LIBRARY_H