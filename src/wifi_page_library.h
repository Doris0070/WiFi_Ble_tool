#ifndef WIFI_PAGE_LIBRARY_H
#define WIFI_PAGE_LIBRARY_H

#include <WiFi.h>
#include <esp_wifi.h>
#include "wifi_settings_jammer_screen.h"

// =====================
// ===== SETTINGS ======
// =====================

#define DEFAULT_CHANNEL 1

struct NetworkInfo {
    String ssid;
    uint8_t bssid[6];
    int channel;
};

extern NetworkInfo targetNetwork;

// === GLOBAL ARRAYS ===
extern const char **wifi_button_texts;
extern int WIFI_BUTTON_COUNT;
extern String *wifi_networks;

// === NEW: Save last clicked index ===
extern int wifi_selected_index;

////////////////////////// WIFI "HOTSPOTS" SCANNER /////////////////////////////

void scanNetworks() {
    if (wifi_networks) {
        delete[] wifi_networks;
        wifi_networks = nullptr;
    }
    if (wifi_button_texts) {
        delete[] wifi_button_texts;
        wifi_button_texts = nullptr;
    }
    WIFI_BUTTON_COUNT = 0;
    Serial.println("[*] Scanning Wi-Fi networks...");
    int n = WiFi.scanNetworks(false, true, false, 300, 13); // Active Scan

    if (n == 0) {
        Serial.println("[!] No networks found!");
        return;
    }

    // Allocate memory (free old if needed)
    if (wifi_networks) delete[] wifi_networks;
    if (wifi_button_texts) delete[] wifi_button_texts;

    wifi_networks = new String[n];
    wifi_button_texts = new const char*[n];

    for (int i = 0; i < n; ++i) {
        wifi_networks[i] = WiFi.SSID(i);
        wifi_button_texts[i] = wifi_networks[i].c_str();
    }
    WIFI_BUTTON_COUNT = n;
}

#endif // WIFI_PAGE_LIBRARY_H