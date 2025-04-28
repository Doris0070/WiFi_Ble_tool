#ifndef WIFI_SETTINGS_JAMMER_SCREEN_H
#define WIFI_SETTINGS_JAMMER_SCREEN_H

#include "lvgl.h"
#include "wifi_page_library.h"
#include "ui/ui.h"

// Screen creator
void wifi_jammer_screen(void) {
    lv_obj_t *wifi_jammer_ssids_selector = lv_obj_create(NULL);
    lv_scr_load(wifi_jammer_ssids_selector); // Load this new screen

    const int button_spacing = 5;
    const int button_height = 50;
    const int button_width_pct = 85;
    int start_y = 0;

    for (uint8_t i = 0; i < WIFI_BUTTON_COUNT; i++) {
        lv_obj_t *btn = lv_btn_create(wifi_jammer_ssids_selector);
        lv_obj_set_size(btn, LV_PCT(button_width_pct), button_height);
        lv_obj_set_pos(btn, LV_PCT((100 - button_width_pct) / 2), start_y + i * (button_height + button_spacing));

        lv_obj_set_user_data(btn, (void*)(uintptr_t)i);
        lv_obj_add_event_cb(btn, wifi_button_event_cb, LV_EVENT_CLICKED, NULL);

        lv_obj_t *label = lv_label_create(btn);
        lv_label_set_text(label, wifi_networks[i].c_str());
        lv_obj_center(label);
    }
}


// ====== FUNCTIONS ======

static void wifi_button_event_cb(lv_event_t *e) {
    lv_obj_t *btn = (lv_obj_t *)lv_event_get_target(e);

    uintptr_t index = (uintptr_t)lv_obj_get_user_data(btn);

    if (index < WIFI_BUTTON_COUNT) {
        wifi_selected_index = index; // Save selected index

        // Save selected network info
        targetNetwork.ssid = wifi_networks[index];
        memcpy(targetNetwork.bssid, WiFi.BSSID(index), 6);
        targetNetwork.channel = WiFi.channel(index);

        Serial.println("[*] Wi-Fi Target Selected:");
        Serial.println("SSID: " + targetNetwork.ssid);
        Serial.print("Channel: "); Serial.println(targetNetwork.channel);
        Serial.print("BSSID: "); Serial.println(WiFi.BSSIDstr(index));

        // 🚀 Switch to jammer screen
        lv_screen_load(objects.wifi_jammer_screen);

        // 🚀 Start jamming immediately
        wifi_jamm();
    }
}

#endif // WIFI_SETTINGS_JAMMER_SCREEN_H
