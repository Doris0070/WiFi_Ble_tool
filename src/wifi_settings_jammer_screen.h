#ifndef WIFI_SETTINGS_JAMMER_SCREEN_H
#define WIFI_SETTINGS_JAMMER_SCREEN_H

#include "lvgl.h"
#include "wifi_page_library.h"

static void wifi_button_event_cb(lv_event_t *e) {
    lv_obj_t *btn = (lv_obj_t *)lv_event_get_target(e);

    // Get stored user data (button index)
    uintptr_t index = (uintptr_t)lv_obj_get_user_data(btn);

    if (index < WIFI_BUTTON_COUNT) {
        wifi_selected_index = index; // Save selected index globally

        // Save into targetNetwork
        targetNetwork.ssid = wifi_networks[index];
        memcpy(targetNetwork.bssid, WiFi.BSSID(index), 6); // get BSSID
        targetNetwork.channel = WiFi.channel(index);       // get channel

        Serial.println("[*] Wi-Fi Selected:");
        Serial.print("SSID: ");
        Serial.println(targetNetwork.ssid);
        Serial.print("Channel: ");
        Serial.println(targetNetwork.channel);
        Serial.print("BSSID: ");
        Serial.println(WiFi.BSSIDstr(index));

        // OPTIONAL: Go back to main screen or show confirmation
    }
}

void wifi_jammer_ssids_selector_screen(void) {
    lv_obj_t *wifi_jammer_ssids_selector = lv_obj_create(NULL);
    lv_scr_load(wifi_jammer_ssids_selector); // Load the new screen

    const int button_spacing = 5;
    const int button_height = 50;
    int start_y = 0;

    for (uint8_t i = 0; i < WIFI_BUTTON_COUNT; i++) {
        lv_obj_t *btn = lv_btn_create(wifi_jammer_ssids_selector);
        lv_obj_set_width(btn, LV_PCT(80));
        lv_obj_set_height(btn, button_height);

        lv_obj_set_x(btn, LV_PCT(10));
        lv_obj_set_y(btn, start_y + i * (button_height + button_spacing));

        lv_obj_add_event_cb(btn, wifi_button_event_cb, LV_EVENT_CLICKED, NULL);

        lv_obj_set_user_data(btn, (void*)(uintptr_t)i); // Store index as user_data!

        lv_obj_t *label = lv_label_create(btn);
        lv_label_set_text(label, wifi_button_texts[i]);
        lv_obj_center(label);
    }
}

#endif // WIFI_SETTINGS_JAMMER_SCREEN_H
