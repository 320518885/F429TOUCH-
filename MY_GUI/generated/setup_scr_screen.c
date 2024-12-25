/*
* Copyright 2024 NXP
* NXP Proprietary. This software is owned or controlled by NXP and may only be used strictly in
* accordance with the applicable license terms. By expressly accepting such terms or by downloading, installing,
* activating and/or otherwise using the software, you are agreeing that you have read, and that you agree to
* comply with and are bound by, such license terms.  If you do not agree to be bound by the applicable license
* terms, then you may not retain, install, activate or otherwise use the software.
*/

#include "lvgl.h"
#include <stdio.h>
#include "gui_guider.h"
#include "events_init.h"
#include "widgets_init.h"
#include "custom.h"


#include "main.h"
#include "tim.h"

extern int nextpage;

extern lv_ui guider_ui;

extern uint8_t lightMode ;// 1: 三色模式, 2: 双色模式 3串口 4LCD


int light_value = 0; // 全局变量，初始值为0
int pwm_value = 0; // 全局变量，初始值设置为0
int period_value = 2000; // 全局变量，初始值设置为2000

static void light_bar_event_cb(lv_event_t * e);
static void pwm_bar_event_cb(lv_event_t * e);
static void period_bar_event_cb(lv_event_t * e);
void update_light_bar(int value);
static void next_page_event_cb(lv_event_t * e);

void update_step_count_label(int step);

void setup_scr_screen(lv_ui *ui)
{
    //Write codes screen
    ui->screen = lv_obj_create(NULL);
    lv_obj_set_size(ui->screen, 320, 240);
    lv_obj_set_scrollbar_mode(ui->screen, LV_SCROLLBAR_MODE_OFF);

    //Write style for screen, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_bg_opa(ui->screen, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes screen_cont_1
    ui->screen_cont_1 = lv_obj_create(ui->screen);
    lv_obj_set_pos(ui->screen_cont_1, 0, 0);
    lv_obj_set_size(ui->screen_cont_1, 320, 240);
    lv_obj_set_scrollbar_mode(ui->screen_cont_1, LV_SCROLLBAR_MODE_OFF);

    //Write style for screen_cont_1, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_border_width(ui->screen_cont_1, 2, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_border_opa(ui->screen_cont_1, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_border_color(ui->screen_cont_1, lv_color_hex(0x2195f6), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_border_side(ui->screen_cont_1, LV_BORDER_SIDE_FULL, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->screen_cont_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->screen_cont_1, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui->screen_cont_1, lv_color_hex(0xffffff), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(ui->screen_cont_1, LV_GRAD_DIR_NONE, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui->screen_cont_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(ui->screen_cont_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->screen_cont_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->screen_cont_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->screen_cont_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes screen_LIGHT_BAR
    ui->screen_LIGHT_BAR = lv_bar_create(ui->screen);
    lv_obj_set_style_anim_time(ui->screen_LIGHT_BAR, 1000, 0);
    lv_bar_set_mode(ui->screen_LIGHT_BAR, LV_BAR_MODE_NORMAL);
    lv_bar_set_range(ui->screen_LIGHT_BAR, 0, 100);
    lv_bar_set_value(ui->screen_LIGHT_BAR, 0, LV_ANIM_OFF);
    lv_obj_set_pos(ui->screen_LIGHT_BAR, 47, 43);
    lv_obj_set_size(ui->screen_LIGHT_BAR, 213, 18);
    lv_obj_add_event_cb(ui->screen_LIGHT_BAR, light_bar_event_cb, LV_EVENT_VALUE_CHANGED, NULL); // 注册回调函数
		
    //Write style for screen_LIGHT_BAR, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_bg_opa(ui->screen_LIGHT_BAR, 60, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui->screen_LIGHT_BAR, lv_color_hex(0x2195f6), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(ui->screen_LIGHT_BAR, LV_GRAD_DIR_NONE, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->screen_LIGHT_BAR, 10, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->screen_LIGHT_BAR, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write style for screen_LIGHT_BAR, Part: LV_PART_INDICATOR, State: LV_STATE_DEFAULT.
    lv_obj_set_style_bg_opa(ui->screen_LIGHT_BAR, 255, LV_PART_INDICATOR|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui->screen_LIGHT_BAR, lv_color_hex(0x2195f6), LV_PART_INDICATOR|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(ui->screen_LIGHT_BAR, LV_GRAD_DIR_NONE, LV_PART_INDICATOR|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->screen_LIGHT_BAR, 10, LV_PART_INDICATOR|LV_STATE_DEFAULT);

    //Write codes screen_LIGHT
    ui->screen_LIGHT = lv_label_create(ui->screen);
    lv_label_set_text(ui->screen_LIGHT, "0\n");
    lv_label_set_long_mode(ui->screen_LIGHT, LV_LABEL_LONG_WRAP);
    lv_obj_set_pos(ui->screen_LIGHT, 103, 19);
    lv_obj_set_size(ui->screen_LIGHT, 94, 16);

    //Write style for screen_LIGHT, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_border_width(ui->screen_LIGHT, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->screen_LIGHT, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(ui->screen_LIGHT, lv_color_hex(0x000000), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->screen_LIGHT, &lv_font_SourceHanSerifSC_Regular_16, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->screen_LIGHT, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_letter_space(ui->screen_LIGHT, 2, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_line_space(ui->screen_LIGHT, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui->screen_LIGHT, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->screen_LIGHT, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui->screen_LIGHT, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->screen_LIGHT, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(ui->screen_LIGHT, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->screen_LIGHT, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->screen_LIGHT, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes screen_PWM_BAR
    ui->screen_PWM_BAR = lv_slider_create(ui->screen);
    lv_slider_set_range(ui->screen_PWM_BAR, 0, 2000);
    lv_slider_set_mode(ui->screen_PWM_BAR, LV_SLIDER_MODE_NORMAL);
    lv_slider_set_value(ui->screen_PWM_BAR, 0, LV_ANIM_OFF);
    lv_obj_set_pos(ui->screen_PWM_BAR, 19, 104);
    lv_obj_set_size(ui->screen_PWM_BAR, 272, 13);
    lv_obj_add_event_cb(ui->screen_PWM_BAR, pwm_bar_event_cb, LV_EVENT_VALUE_CHANGED, NULL); // 注册回调函数
		
    //Write style for screen_PWM_BAR, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_bg_opa(ui->screen_PWM_BAR, 60, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui->screen_PWM_BAR, lv_color_hex(0x2195f6), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(ui->screen_PWM_BAR, LV_GRAD_DIR_NONE, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->screen_PWM_BAR, 50, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_outline_width(ui->screen_PWM_BAR, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->screen_PWM_BAR, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write style for screen_PWM_BAR, Part: LV_PART_INDICATOR, State: LV_STATE_DEFAULT.
    lv_obj_set_style_bg_opa(ui->screen_PWM_BAR, 255, LV_PART_INDICATOR|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui->screen_PWM_BAR, lv_color_hex(0x2195f6), LV_PART_INDICATOR|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(ui->screen_PWM_BAR, LV_GRAD_DIR_NONE, LV_PART_INDICATOR|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->screen_PWM_BAR, 50, LV_PART_INDICATOR|LV_STATE_DEFAULT);

    //Write style for screen_PWM_BAR, Part: LV_PART_KNOB, State: LV_STATE_DEFAULT.
    lv_obj_set_style_bg_opa(ui->screen_PWM_BAR, 255, LV_PART_KNOB|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui->screen_PWM_BAR, lv_color_hex(0x2195f6), LV_PART_KNOB|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(ui->screen_PWM_BAR, LV_GRAD_DIR_NONE, LV_PART_KNOB|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->screen_PWM_BAR, 50, LV_PART_KNOB|LV_STATE_DEFAULT);

    //Write codes screen_PWM
    ui->screen_PWM = lv_label_create(ui->screen);
    lv_label_set_text(ui->screen_PWM, "0\n\n");
    lv_label_set_long_mode(ui->screen_PWM, LV_LABEL_LONG_WRAP);
    lv_obj_set_pos(ui->screen_PWM, 103, 76);
    lv_obj_set_size(ui->screen_PWM, 100, 33);

    //Write style for screen_PWM, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_border_width(ui->screen_PWM, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->screen_PWM, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(ui->screen_PWM, lv_color_hex(0x000000), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->screen_PWM, &lv_font_SourceHanSerifSC_Regular_16, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->screen_PWM, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_letter_space(ui->screen_PWM, 2, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_line_space(ui->screen_PWM, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui->screen_PWM, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->screen_PWM, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui->screen_PWM, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->screen_PWM, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(ui->screen_PWM, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->screen_PWM, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->screen_PWM, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes screen_PERIOD_BAR
    ui->screen_PERIOD_BAR = lv_slider_create(ui->screen);
    lv_slider_set_range(ui->screen_PERIOD_BAR, 0, 4000);
    lv_slider_set_mode(ui->screen_PERIOD_BAR, LV_SLIDER_MODE_NORMAL);
    lv_slider_set_value(ui->screen_PERIOD_BAR, 2000, LV_ANIM_OFF);
    lv_obj_set_pos(ui->screen_PERIOD_BAR, 19, 166);
    lv_obj_set_size(ui->screen_PERIOD_BAR, 272, 14);
    lv_obj_add_event_cb(ui->screen_PERIOD_BAR, period_bar_event_cb, LV_EVENT_VALUE_CHANGED, NULL); // 注册回调函数	
		
    //Write style for screen_PERIOD_BAR, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_bg_opa(ui->screen_PERIOD_BAR, 60, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui->screen_PERIOD_BAR, lv_color_hex(0x2195f6), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(ui->screen_PERIOD_BAR, LV_GRAD_DIR_NONE, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->screen_PERIOD_BAR, 50, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_outline_width(ui->screen_PERIOD_BAR, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->screen_PERIOD_BAR, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write style for screen_PERIOD_BAR, Part: LV_PART_INDICATOR, State: LV_STATE_DEFAULT.
    lv_obj_set_style_bg_opa(ui->screen_PERIOD_BAR, 255, LV_PART_INDICATOR|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui->screen_PERIOD_BAR, lv_color_hex(0x2195f6), LV_PART_INDICATOR|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(ui->screen_PERIOD_BAR, LV_GRAD_DIR_NONE, LV_PART_INDICATOR|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->screen_PERIOD_BAR, 50, LV_PART_INDICATOR|LV_STATE_DEFAULT);

    //Write style for screen_PERIOD_BAR, Part: LV_PART_KNOB, State: LV_STATE_DEFAULT.
    lv_obj_set_style_bg_opa(ui->screen_PERIOD_BAR, 255, LV_PART_KNOB|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui->screen_PERIOD_BAR, lv_color_hex(0x2195f6), LV_PART_KNOB|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(ui->screen_PERIOD_BAR, LV_GRAD_DIR_NONE, LV_PART_KNOB|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->screen_PERIOD_BAR, 50, LV_PART_KNOB|LV_STATE_DEFAULT);

    //Write codes screen_PERIOD
    ui->screen_PERIOD = lv_label_create(ui->screen);
    lv_label_set_text(ui->screen_PERIOD, "2000\n");
    lv_label_set_long_mode(ui->screen_PERIOD, LV_LABEL_LONG_WRAP);
    lv_obj_set_pos(ui->screen_PERIOD, 103, 134);
    lv_obj_set_size(ui->screen_PERIOD, 100, 32);

    //Write style for screen_PERIOD, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_border_width(ui->screen_PERIOD, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->screen_PERIOD, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(ui->screen_PERIOD, lv_color_hex(0x000000), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->screen_PERIOD, &lv_font_SourceHanSerifSC_Regular_16, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->screen_PERIOD, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_letter_space(ui->screen_PERIOD, 2, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_line_space(ui->screen_PERIOD, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui->screen_PERIOD, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->screen_PERIOD, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui->screen_PERIOD, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->screen_PERIOD, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(ui->screen_PERIOD, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->screen_PERIOD, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->screen_PERIOD, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes screen_nextpage
    ui->screen_nextpage = lv_imgbtn_create(ui->screen);
    lv_obj_add_flag(ui->screen_nextpage, LV_OBJ_FLAG_CHECKABLE);
    lv_imgbtn_set_src(ui->screen_nextpage, LV_IMGBTN_STATE_RELEASED, NULL, &_nextpage_alpha_41x33, NULL);
    lv_imgbtn_set_src(ui->screen_nextpage, LV_IMGBTN_STATE_PRESSED, NULL, &_nextpage_yellow_alpha_41x33, NULL);
    ui->screen_nextpage_label = lv_label_create(ui->screen_nextpage);
    lv_label_set_text(ui->screen_nextpage_label, "");
    lv_label_set_long_mode(ui->screen_nextpage_label, LV_LABEL_LONG_WRAP);
    lv_obj_align(ui->screen_nextpage_label, LV_ALIGN_CENTER, 0, 0);
    lv_obj_set_style_pad_all(ui->screen_nextpage, 0, LV_STATE_DEFAULT);
    lv_obj_set_pos(ui->screen_nextpage, 264, 199);
    lv_obj_set_size(ui->screen_nextpage, 41, 33);
		lv_obj_add_event_cb(ui->screen_nextpage, next_page_event_cb, LV_EVENT_CLICKED, NULL);//回调事件
		
    //Write style for screen_nextpage, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_text_color(ui->screen_nextpage, lv_color_hex(0x000000), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->screen_nextpage, &lv_font_SourceHanSerifSC_Regular_12, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->screen_nextpage, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui->screen_nextpage, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->screen_nextpage, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_clip_corner(ui->screen_nextpage, true, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->screen_nextpage, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write style for screen_nextpage, Part: LV_PART_MAIN, State: LV_STATE_PRESSED.
    lv_obj_set_style_img_recolor_opa(ui->screen_nextpage, 0, LV_PART_MAIN|LV_STATE_PRESSED);
    lv_obj_set_style_img_opa(ui->screen_nextpage, 255, LV_PART_MAIN|LV_STATE_PRESSED);
    lv_obj_set_style_text_color(ui->screen_nextpage, lv_color_hex(0xFF33FF), LV_PART_MAIN|LV_STATE_PRESSED);
    lv_obj_set_style_text_font(ui->screen_nextpage, &lv_font_SourceHanSerifSC_Regular_12, LV_PART_MAIN|LV_STATE_PRESSED);
    lv_obj_set_style_text_opa(ui->screen_nextpage, 255, LV_PART_MAIN|LV_STATE_PRESSED);
    lv_obj_set_style_shadow_width(ui->screen_nextpage, 0, LV_PART_MAIN|LV_STATE_PRESSED);

    //Write style for screen_nextpage, Part: LV_PART_MAIN, State: LV_STATE_CHECKED.
    lv_obj_set_style_img_recolor_opa(ui->screen_nextpage, 0, LV_PART_MAIN|LV_STATE_CHECKED);
    lv_obj_set_style_img_opa(ui->screen_nextpage, 255, LV_PART_MAIN|LV_STATE_CHECKED);
    lv_obj_set_style_text_color(ui->screen_nextpage, lv_color_hex(0xFF33FF), LV_PART_MAIN|LV_STATE_CHECKED);
    lv_obj_set_style_text_font(ui->screen_nextpage, &lv_font_SourceHanSerifSC_Regular_12, LV_PART_MAIN|LV_STATE_CHECKED);
    lv_obj_set_style_text_opa(ui->screen_nextpage, 255, LV_PART_MAIN|LV_STATE_CHECKED);
    lv_obj_set_style_shadow_width(ui->screen_nextpage, 0, LV_PART_MAIN|LV_STATE_CHECKED);

    //Write style for screen_nextpage, Part: LV_PART_MAIN, State: LV_IMGBTN_STATE_RELEASED.
    lv_obj_set_style_img_recolor_opa(ui->screen_nextpage, 0, LV_PART_MAIN|LV_IMGBTN_STATE_RELEASED);
    lv_obj_set_style_img_opa(ui->screen_nextpage, 255, LV_PART_MAIN|LV_IMGBTN_STATE_RELEASED);

    //Write codes screen_label_1
    ui->screen_label_1 = lv_label_create(ui->screen);
    lv_label_set_text(ui->screen_label_1, "步数：0");
    lv_label_set_long_mode(ui->screen_label_1, LV_LABEL_LONG_WRAP);
    lv_obj_set_pos(ui->screen_label_1, 92, 199);
    lv_obj_set_size(ui->screen_label_1, 100, 32);

    //Write style for screen_label_1, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_border_width(ui->screen_label_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->screen_label_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(ui->screen_label_1, lv_color_hex(0x000000), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->screen_label_1, &lv_font_SourceHanSerifSC_Regular_16, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->screen_label_1, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_letter_space(ui->screen_label_1, 2, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_line_space(ui->screen_label_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui->screen_label_1, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->screen_label_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui->screen_label_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->screen_label_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(ui->screen_label_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->screen_label_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->screen_label_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

    //The custom code of screen.


    //Update current screen layout.
    lv_obj_update_layout(ui->screen);

    //Init events for screen.
    events_init_screen(ui);
}

void update_step_count_label(int step) {
    char step_text[20];
    snprintf(step_text, sizeof(step_text), "步数：%d", step);  // 格式化步数
    lv_label_set_text(guider_ui.screen_label_1, step_text);  // 更新标签文本
}

static void light_bar_event_cb(lv_event_t * e) {
    if (lv_event_get_code(e) == LV_EVENT_VALUE_CHANGED) {
        lv_obj_t * bar = lv_event_get_target(e);
        light_value = lv_bar_get_value(bar); // 更新全局变量的值
        lv_label_set_text_fmt(guider_ui.screen_LIGHT, "%d", light_value); // 更新标签的文本
    }
}

static void pwm_bar_event_cb(lv_event_t * e) {
    if (lv_event_get_code(e) == LV_EVENT_VALUE_CHANGED) {
        lv_obj_t * slider = lv_event_get_target(e);
        pwm_value = lv_slider_get_value(slider); // 更新全局变量的值
        lv_label_set_text_fmt(guider_ui.screen_PWM, "%d", pwm_value); // 更新标签的文本
			if(lightMode == 4){
				__HAL_TIM_SetCompare(&htim9, TIM_CHANNEL_1, pwm_value);
				__HAL_TIM_SetCompare(&htim8, TIM_CHANNEL_3, pwm_value);
			}
    }
}

static void period_bar_event_cb(lv_event_t * e) {
    if (lv_event_get_code(e) == LV_EVENT_VALUE_CHANGED) {
        lv_obj_t * slider = lv_event_get_target(e);
        period_value = lv_slider_get_value(slider); // 更新全局变量的值
        lv_label_set_text_fmt(guider_ui.screen_PERIOD, "%d", period_value); // 更新标签的文本

        // 更新 PWM_BAR 的范围
        lv_slider_set_range(guider_ui.screen_PWM_BAR, 0, period_value);

        // 确保 PWM_BAR 的当前值不超过新的最大值并且更新pwm值
        int16_t current_pwm_value = lv_slider_get_value(guider_ui.screen_PWM_BAR);
        if (current_pwm_value > period_value) {
            lv_slider_set_value(guider_ui.screen_PWM_BAR, period_value, LV_ANIM_ON);
            lv_label_set_text_fmt(guider_ui.screen_PWM, "%d", period_value); // 更新标签的文本
        }

        // 更新 TIM8 和 TIM9 的 ARR 值
        // 假设 htim8 和 htim9 已经被正确初始化
        __HAL_TIM_SET_AUTORELOAD(&htim8, period_value - 1);
        __HAL_TIM_SET_AUTORELOAD(&htim9, period_value - 1);

        if(lightMode == 4){
            // 更新 PWM 值
            __HAL_TIM_SetCompare(&htim9, TIM_CHANNEL_1, current_pwm_value);
            __HAL_TIM_SetCompare(&htim8, TIM_CHANNEL_3, current_pwm_value);
        }
    }
}

//换页键被点击的时候
static void next_page_event_cb(lv_event_t * e) {
    if (lv_event_get_code(e) == LV_EVENT_CLICKED) {
        // 换页逻辑
				nextpage = 2;//换页后为图表页面
    }
}


void update_light_bar(int value) {
    lv_bar_set_value(guider_ui.screen_LIGHT_BAR, value, LV_ANIM_OFF); // 更新滑块的值，无动画
    lv_label_set_text_fmt(guider_ui.screen_LIGHT, "%d", value); // 更新标签的文本
}
