#ifndef WIFI_SETTINGS_JAMMER_SCREEN_H
#define WIFI_SETTINGS_JAMMER_SCREEN_H

#include "lvgl.h"
#include "wifi_page_library.h"
#include "ui/ui.h"

// ===== Forward Declarations =====
static void wifi_button_event_cb_1(lv_event_t *e);
static void wifi_button_event_cb_2(lv_event_t *e);
static void back_button_event_cb(lv_event_t *e);

// ===== Screen Creators =====

void wifi_jammer_screen_jamm(void) {
    lv_scr_load(objects.wifi_jammer_ssids_selector); // Load screen
    lv_obj_clean(objects.wifi_jammer_ssids_selector);

    // ---- Title Label ----
    lv_obj_t *title_label = lv_label_create(objects.wifi_jammer_ssids_selector);
    lv_label_set_text(title_label, "Choose Network");
    lv_obj_set_style_text_font(title_label, &lv_font_montserrat_20, 0);
    lv_obj_align(title_label, LV_ALIGN_TOP_MID, 0, 10);

    if (WIFI_BUTTON_COUNT == 0) {
        // ---- No Networks Found ----
        lv_obj_t *no_networks_label = lv_label_create(objects.wifi_jammer_ssids_selector);
        lv_label_set_text(no_networks_label, "No networks found");
        lv_obj_set_style_text_font(no_networks_label, &lv_font_montserrat_20, 0);
        lv_obj_align(no_networks_label, LV_ALIGN_CENTER, 0, -20);

        // ---- Back Button ----
        lv_obj_t *back_btn = lv_btn_create(objects.wifi_jammer_ssids_selector);
        lv_obj_set_size(back_btn, 120, 40);
        lv_obj_align(back_btn, LV_ALIGN_CENTER, 0, 30);
        lv_obj_add_event_cb(back_btn, back_button_event_cb, LV_EVENT_CLICKED, NULL);

        lv_obj_t *back_label = lv_label_create(back_btn);
        lv_label_set_text(back_label, "Back");
        lv_obj_center(back_label);

    } else {
        // ---- Network Buttons ----
        const int button_spacing = 5;
        const int button_height = 50;
        const int button_width_pct = 85;
        int start_y = 50; // Start below title

        for (uint8_t i = 0; i < WIFI_BUTTON_COUNT; i++) {
            lv_obj_t *btn = lv_btn_create(objects.wifi_jammer_ssids_selector);
            lv_obj_set_size(btn, LV_PCT(button_width_pct), button_height);
            lv_obj_set_pos(btn, LV_PCT((100 - button_width_pct) / 2), start_y + i * (button_height + button_spacing));
            lv_obj_set_user_data(btn, (void*)(uintptr_t)i);
            lv_obj_add_event_cb(btn, wifi_button_event_cb_1, LV_EVENT_CLICKED, NULL);

            lv_obj_t *label = lv_label_create(btn);
            lv_label_set_text(label, wifi_networks[i].c_str());
            lv_obj_center(label);
        }
    }
}

void wifi_jammer_screen_dos(void) {
    lv_scr_load(objects.wifi_jammer_ssids_selector); // Load screen
    lv_obj_clean(objects.wifi_jammer_ssids_selector);

    // ---- Title Label ----
    lv_obj_t *title_label = lv_label_create(objects.wifi_jammer_ssids_selector);
    lv_label_set_text(title_label, "Choose Network");
    lv_obj_set_style_text_font(title_label, &lv_font_montserrat_20, 0);
    lv_obj_align(title_label, LV_ALIGN_TOP_MID, 0, 10);

    if (WIFI_BUTTON_COUNT == 0) {
        // ---- No Networks Found Label ----
        lv_obj_t *no_networks_label = lv_label_create(objects.wifi_jammer_ssids_selector);
        lv_label_set_text(no_networks_label, "No networks found");
        lv_obj_set_style_text_font(no_networks_label, &lv_font_montserrat_20, 0);
        lv_obj_align(no_networks_label, LV_ALIGN_CENTER, 0, -40); // Higher position
    
        // ---- Back Button ----
        lv_obj_t *back_btn = lv_btn_create(objects.wifi_jammer_ssids_selector);
        lv_obj_set_size(back_btn, 120, 40);
        lv_obj_align(back_btn, LV_ALIGN_CENTER, 0, 20); // Lower than label
        lv_obj_add_event_cb(back_btn, back_button_event_cb, LV_EVENT_CLICKED, NULL);
    
        lv_obj_t *back_label = lv_label_create(back_btn);
        lv_label_set_text(back_label, "Back");
        lv_obj_center(back_label);
    } else {
        // ---- Network Buttons ----
        const int button_spacing = 5;
        const int button_height = 50;
        const int button_width_pct = 85;
        int start_y = 50; // Start below title

        for (uint8_t i = 0; i < WIFI_BUTTON_COUNT; i++) {
            lv_obj_t *btn = lv_btn_create(objects.wifi_jammer_ssids_selector);
            lv_obj_set_size(btn, LV_PCT(button_width_pct), button_height);
            lv_obj_set_pos(btn, LV_PCT((100 - button_width_pct) / 2), start_y + i * (button_height + button_spacing));
            lv_obj_set_user_data(btn, (void*)(uintptr_t)i);
            lv_obj_add_event_cb(btn, wifi_button_event_cb_2, LV_EVENT_CLICKED, NULL);

            lv_obj_t *label = lv_label_create(btn);
            lv_label_set_text(label, wifi_networks[i].c_str());
            lv_obj_center(label);
        }
    }
}

// ===== Event Callbacks =====

static void wifi_button_event_cb_1(lv_event_t *e) {
    lv_obj_t *btn = (lv_obj_t *)lv_event_get_target(e);
    uintptr_t index = (uintptr_t)lv_obj_get_user_data(btn);

    if (index < WIFI_BUTTON_COUNT) {
        wifi_selected_index = index;

        targetNetwork.ssid = wifi_networks[index];
        memcpy(targetNetwork.bssid, WiFi.BSSID(index), 6);
        targetNetwork.channel = WiFi.channel(index);

        Serial.println("[*] Wi-Fi Target Selected:");
        Serial.println("SSID: " + targetNetwork.ssid);
        Serial.print("Channel: "); Serial.println(targetNetwork.channel);
        Serial.print("BSSID: "); Serial.println(WiFi.BSSIDstr(index));

        WiFi.scanDelete();
        lv_screen_load(objects.wifi_jammer_screen);

        wifi_jamm();
    }
}

static void wifi_button_event_cb_2(lv_event_t *e) {
    lv_obj_t *btn = (lv_obj_t *)lv_event_get_target(e);
    uintptr_t index = (uintptr_t)lv_obj_get_user_data(btn);

    if (index < WIFI_BUTTON_COUNT) {
        wifi_selected_index = index;

        targetNetwork.ssid = wifi_networks[index];
        memcpy(targetNetwork.bssid, WiFi.BSSID(index), 6);
        targetNetwork.channel = WiFi.channel(index);

        Serial.println("[*] Wi-Fi Target Selected:");
        Serial.println("SSID: " + targetNetwork.ssid);
        Serial.print("Channel: "); Serial.println(targetNetwork.channel);
        Serial.print("BSSID: "); Serial.println(WiFi.BSSIDstr(index));

        WiFi.scanDelete();
        lv_screen_load(objects.wifi_jammer_screen);

        wifi_dos();
    }
}

static void back_button_event_cb(lv_event_t *e) {
    lv_screen_load(objects.wifi_settings); // Go back to WiFi settings
}

#endif // WIFI_SETTINGS_JAMMER_SCREEN_H