#ifndef EEZ_LVGL_UI_SCREENS_H
#define EEZ_LVGL_UI_SCREENS_H

#include <lvgl.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef struct _objects_t {
    lv_obj_t *main;
    lv_obj_t *nastavitve;
    lv_obj_t *img_h_hand;
    lv_obj_t *img_m_hand;
    lv_obj_t *img_s_hand;
    lv_obj_t *b1;
    lv_obj_t *b1_label;
    lv_obj_t *wi_fi_bli;
    lv_obj_t *ssid_label;
    lv_obj_t *ssid_pass_label;
    lv_obj_t *kbd;
    lv_obj_t *b_connect;
    lv_obj_t *b_back;
    lv_obj_t *tb_ssid_pass;
    lv_obj_t *tb_ssid_text;
} objects_t;

extern objects_t objects;

enum ScreensEnum {
    SCREEN_ID_MAIN = 1,
    SCREEN_ID_NASTAVITVE = 2,
};

void create_screen_main();
void tick_screen_main();

void create_screen_nastavitve();
void tick_screen_nastavitve();

void tick_screen_by_id(enum ScreensEnum screenId);
void tick_screen(int screen_index);

void create_screens();


#ifdef __cplusplus
}
#endif

#endif /*EEZ_LVGL_UI_SCREENS_H*/