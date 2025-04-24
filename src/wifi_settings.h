#include "Arduino.h"
#include <WiFi.h>
#include <lvgl.h>
#include "ui/ui.h"
#include "Preferences.h"

char wifi_ssid[32];
char wifi_password[32];

Preferences NastavitveWiFi;

void fix_wifi_ui_textarea() {
    // fix nastavitve ui
    lv_textarea_set_text(objects.tb_ssid_text, wifi_ssid);
    lv_textarea_set_text(objects.tb_ssid_pass, wifi_password);
}

void wifi_setup() {
    // Initialize WiFi
    WiFi.mode(WIFI_STA);
    WiFi.setSleep(false);
    WiFi.setAutoReconnect(true);
    WiFi.setHostname("ESP32-Clock");

    NastavitveWiFi.begin("wifi", false);
    NastavitveWiFi.getString("ssid", wifi_ssid, sizeof(wifi_ssid));
    NastavitveWiFi.getString("password", wifi_password, sizeof(wifi_password));
    if (strlen(wifi_ssid) == 0 || strlen(wifi_password) == 0) {
        Serial.println("No WiFi credentials found in preferences");
        //! unsecure, default credentials
        strncpy(wifi_ssid, "VegovaIoT", sizeof(wifi_ssid));
        strncpy(wifi_password, "VegovaIoT", sizeof(wifi_password));
    } else {
        Serial.printf("WiFi credentials found in preferences: %s, %s\n", wifi_ssid, wifi_password);
    }
    NastavitveWiFi.end();
    // nastavitve v uporabniškem vmesniku popravimo potem, ko bomo klicali ui_init()

    // poveži se na WiFi
    WiFi.begin(wifi_ssid, wifi_password);
    Serial.printf("Connecting to WiFi SSID: %s with password %s\n", wifi_ssid, wifi_password);

    Serial.println("WiFi setup done");
    Serial.print("Connecting to WiFi...");
    while (WiFi.status() != WL_CONNECTED) {
        delay(1000);
        Serial.print(".");
    }
    Serial.printf("Connected to WiFi\nIP address: %s\n", WiFi.localIP().toString().c_str());
}