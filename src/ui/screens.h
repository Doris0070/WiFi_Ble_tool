#ifndef EEZ_LVGL_UI_SCREENS_H
#define EEZ_LVGL_UI_SCREENS_H

#include <lvgl.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef struct _objects_t {
    lv_obj_t *main;
    lv_obj_t *wifi_settings;
    lv_obj_t *ble_settings;
    lv_obj_t *ble_button;
    lv_obj_t *ble_button_label;
    lv_obj_t *wifi_button;
    lv_obj_t *wifi_button_label;
} objects_t;

extern objects_t objects;

enum ScreensEnum {
    SCREEN_ID_MAIN = 1,
    SCREEN_ID_WIFI_SETTINGS = 2,
    SCREEN_ID_BLE_SETTINGS = 3,
};

void create_screen_main();
void tick_screen_main();

void create_screen_wifi_settings();
void tick_screen_wifi_settings();

void create_screen_ble_settings();
void tick_screen_ble_settings();

void tick_screen_by_id(enum ScreensEnum screenId);
void tick_screen(int screen_index);

void create_screens();


#ifdef __cplusplus
}
#endif

#endif /*EEZ_LVGL_UI_SCREENS_H*/