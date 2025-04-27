#ifndef WIFI_PAGE_LIBRARY_H
#define WIFI_PAGE_LIBRARY_H

#include <WiFi.h>
#include <esp_wifi.h>

// =============== SETTINGS ===============
#define DEFAULT_CHANNEL 1

struct NetworkInfo {
    String ssid;
    uint8_t bssid[6];
    int channel;
};

// ===== GLOBALS =====
extern NetworkInfo targetNetwork;
extern const char **wifi_button_texts;
extern String *wifi_networks;
extern int WIFI_BUTTON_COUNT;
extern int wifi_selected_index;

// =============== FUNCTIONS ===============

// Scan for Wi-Fi networks
void scanNetworks();

// Start Wi-Fi Jamming
void wifi_jamm();

// Start Wi-Fi DoS attack
void wifi_dos();

// Stop any jamming
void wifi_jamm_stop();

#endif // WIFI_PAGE_LIBRARY_H
