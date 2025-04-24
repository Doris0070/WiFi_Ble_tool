#include <string.h>

#include "screens.h"
#include "images.h"
#include "fonts.h"
#include "actions.h"
#include "vars.h"
#include "styles.h"
#include "ui.h"

#include <string.h>

objects_t objects;
lv_obj_t *tick_value_change_obj;

void create_screen_main() {
    lv_obj_t *obj = lv_obj_create(0);
    objects.main = obj;
    lv_obj_set_pos(obj, 0, 0);
    lv_obj_set_size(obj, 320, 240);
    lv_obj_set_style_bg_color(obj, lv_color_hex(0xff8b0505), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_color(obj, lv_color_hex(0xff250101), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(obj, LV_GRAD_DIR_VER, LV_PART_MAIN | LV_STATE_DEFAULT);
    {
        lv_obj_t *parent_obj = obj;
        {
            lv_obj_t *obj = lv_obj_create(parent_obj);
            lv_obj_set_pos(obj, 18, 4);
            lv_obj_set_size(obj, 233, 233);
            lv_obj_set_style_pad_left(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_pad_top(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_pad_right(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_pad_bottom(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_bg_opa(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_border_width(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_radius(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
            {
                lv_obj_t *parent_obj = obj;
                {
                    lv_obj_t *obj = lv_img_create(parent_obj);
                    lv_obj_set_pos(obj, 2, 2);
                    lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                    lv_img_set_src(obj, &img_analog_clock_template);
                }
                {
                    // img_h_hand
                    lv_obj_t *obj = lv_img_create(parent_obj);
                    objects.img_h_hand = obj;
                    lv_obj_set_pos(obj, 109, 56);
                    lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                    lv_img_set_src(obj, &img_hour_hand);
                    lv_image_set_inner_align(obj, LV_IMAGE_ALIGN_TOP_RIGHT);
                    lv_obj_set_style_transform_pivot_x(obj, 7, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_transform_pivot_y(obj, 64, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_transform_rotation(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                }
                {
                    // img_m_hand
                    lv_obj_t *obj = lv_img_create(parent_obj);
                    objects.img_m_hand = obj;
                    lv_obj_set_pos(obj, 110, 38);
                    lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                    lv_img_set_src(obj, &img_minute_hand);
                    lv_obj_set_style_transform_pivot_x(obj, 6, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_transform_pivot_y(obj, 82, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_transform_rotation(obj, 400, LV_PART_MAIN | LV_STATE_DEFAULT);
                }
                {
                    // img_s_hand
                    lv_obj_t *obj = lv_img_create(parent_obj);
                    objects.img_s_hand = obj;
                    lv_obj_set_pos(obj, 113, 30);
                    lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                    lv_img_set_src(obj, &img_second_hand);
                    lv_obj_set_style_transform_pivot_x(obj, 3.5, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_transform_pivot_y(obj, 90, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_transform_rotation(obj, 1800, LV_PART_MAIN | LV_STATE_DEFAULT);
                }
                {
                    lv_obj_t *obj = lv_img_create(parent_obj);
                    lv_obj_set_pos(obj, 109, 113);
                    lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                    lv_img_set_src(obj, &img_analog_middle_template);
                }
            }
        }
        {
            // B1
            lv_obj_t *obj = lv_btn_create(parent_obj);
            objects.b1 = obj;
            lv_obj_set_pos(obj, 260, 181);
            lv_obj_set_size(obj, 50, 50);
            {
                lv_obj_t *parent_obj = obj;
                {
                    // B1_label
                    lv_obj_t *obj = lv_label_create(parent_obj);
                    objects.b1_label = obj;
                    lv_obj_set_pos(obj, 0, 0);
                    lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                    lv_label_set_text(obj, "\uf013");
                    lv_obj_set_style_align(obj, LV_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
                }
            }
        }
        {
            // wi_fi_bli
            lv_obj_t *obj = lv_label_create(parent_obj);
            objects.wi_fi_bli = obj;
            lv_obj_set_pos(obj, 293, 4);
            lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
            lv_label_set_long_mode(obj, LV_LABEL_LONG_CLIP);
            lv_label_set_text(obj, "\uf1eb");
            lv_obj_set_style_text_color(obj, lv_color_hex(0xffffffff), LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_text_align(obj, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
        }
    }
    
    tick_screen_main();
}

void tick_screen_main() {
}

void create_screen_nastavitve() {
    lv_obj_t *obj = lv_obj_create(0);
    objects.nastavitve = obj;
    lv_obj_set_pos(obj, 0, 0);
    lv_obj_set_size(obj, 320, 240);
    {
        lv_obj_t *parent_obj = obj;
        {
            // ssid_label
            lv_obj_t *obj = lv_label_create(parent_obj);
            objects.ssid_label = obj;
            lv_obj_set_pos(obj, 0, 0);
            lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
            lv_label_set_text(obj, "SSID");
        }
        {
            // ssid_pass_label
            lv_obj_t *obj = lv_label_create(parent_obj);
            objects.ssid_pass_label = obj;
            lv_obj_set_pos(obj, 0, 65);
            lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
            lv_label_set_text(obj, "PASSWORD");
        }
        {
            // kbd
            lv_obj_t *obj = lv_keyboard_create(parent_obj);
            objects.kbd = obj;
            lv_obj_set_pos(obj, 0, 120);
            lv_obj_set_size(obj, 320, 120);
            lv_obj_set_style_align(obj, LV_ALIGN_DEFAULT, LV_PART_MAIN | LV_STATE_DEFAULT);
        }
        {
            // b_connect
            lv_obj_t *obj = lv_btn_create(parent_obj);
            objects.b_connect = obj;
            lv_obj_set_pos(obj, 257, 81);
            lv_obj_set_size(obj, 63, 35);
            {
                lv_obj_t *parent_obj = obj;
                {
                    lv_obj_t *obj = lv_label_create(parent_obj);
                    lv_obj_set_pos(obj, 0, 0);
                    lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                    lv_label_set_text(obj, "Connect");
                    lv_obj_set_style_align(obj, LV_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_text_font(obj, &lv_font_montserrat_10, LV_PART_MAIN | LV_STATE_DEFAULT);
                }
            }
        }
        {
            // b_back
            lv_obj_t *obj = lv_btn_create(parent_obj);
            objects.b_back = obj;
            lv_obj_set_pos(obj, 258, 17);
            lv_obj_set_size(obj, 63, 35);
            {
                lv_obj_t *parent_obj = obj;
                {
                    lv_obj_t *obj = lv_label_create(parent_obj);
                    lv_obj_set_pos(obj, 0, 0);
                    lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                    lv_label_set_text(obj, "Back");
                    lv_obj_set_style_align(obj, LV_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_text_font(obj, &lv_font_montserrat_10, LV_PART_MAIN | LV_STATE_DEFAULT);
                }
            }
        }
        {
            // tb_ssid_pass
            lv_obj_t *obj = lv_textarea_create(parent_obj);
            objects.tb_ssid_pass = obj;
            lv_obj_set_pos(obj, 0, 81);
            lv_obj_set_size(obj, 235, 35);
            lv_textarea_set_max_length(obj, 32);
            lv_textarea_set_placeholder_text(obj, "SSID password");
            lv_textarea_set_one_line(obj, true);
            lv_textarea_set_password_mode(obj, true);
        }
        {
            // tb_ssid_text
            lv_obj_t *obj = lv_textarea_create(parent_obj);
            objects.tb_ssid_text = obj;
            lv_obj_set_pos(obj, 1, 17);
            lv_obj_set_size(obj, 235, 36);
            lv_textarea_set_max_length(obj, 32);
            lv_textarea_set_placeholder_text(obj, "SSID");
            lv_textarea_set_one_line(obj, true);
            lv_textarea_set_password_mode(obj, false);
        }
    }
    
    tick_screen_nastavitve();
}

void tick_screen_nastavitve() {
}



typedef void (*tick_screen_func_t)();
tick_screen_func_t tick_screen_funcs[] = {
    tick_screen_main,
    tick_screen_nastavitve,
};
void tick_screen(int screen_index) {
    tick_screen_funcs[screen_index]();
}
void tick_screen_by_id(enum ScreensEnum screenId) {
    tick_screen_funcs[screenId - 1]();
}

void create_screens() {
    lv_disp_t *dispp = lv_disp_get_default();
    lv_theme_t *theme = lv_theme_default_init(dispp, lv_palette_main(LV_PALETTE_BLUE), lv_palette_main(LV_PALETTE_RED), false, LV_FONT_DEFAULT);
    lv_disp_set_theme(dispp, theme);
    
    create_screen_main();
    create_screen_nastavitve();
}
