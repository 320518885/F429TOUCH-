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


extern int nextpage;

void update_chart_value(int var_value);
static void next_page_1_event_cb(lv_event_t * e);

void setup_scr_screen_1(lv_ui *ui)
{
    //Write codes screen_1
    ui->screen_1 = lv_obj_create(NULL);
    lv_obj_set_size(ui->screen_1, 320, 240);
    lv_obj_set_scrollbar_mode(ui->screen_1, LV_SCROLLBAR_MODE_OFF);

    //Write style for screen_1, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_bg_opa(ui->screen_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes screen_1_cont_1
    ui->screen_1_cont_1 = lv_obj_create(ui->screen_1);
    lv_obj_set_pos(ui->screen_1_cont_1, 0, 0);
    lv_obj_set_size(ui->screen_1_cont_1, 320, 240);
    lv_obj_set_scrollbar_mode(ui->screen_1_cont_1, LV_SCROLLBAR_MODE_OFF);

    //Write style for screen_1_cont_1, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_border_width(ui->screen_1_cont_1, 2, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_border_opa(ui->screen_1_cont_1, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_border_color(ui->screen_1_cont_1, lv_color_hex(0x2195f6), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_border_side(ui->screen_1_cont_1, LV_BORDER_SIDE_FULL, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->screen_1_cont_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->screen_1_cont_1, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui->screen_1_cont_1, lv_color_hex(0xffffff), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(ui->screen_1_cont_1, LV_GRAD_DIR_NONE, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui->screen_1_cont_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(ui->screen_1_cont_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->screen_1_cont_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->screen_1_cont_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->screen_1_cont_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes screen_1_lastpage
    ui->screen_1_lastpage = lv_imgbtn_create(ui->screen_1);
    lv_obj_add_flag(ui->screen_1_lastpage, LV_OBJ_FLAG_CHECKABLE);
    lv_imgbtn_set_src(ui->screen_1_lastpage, LV_IMGBTN_STATE_RELEASED, NULL, &_nextpage_alpha_41x33, NULL);
    lv_imgbtn_set_src(ui->screen_1_lastpage, LV_IMGBTN_STATE_PRESSED, NULL, &_nextpage_yellow_alpha_41x33, NULL);
    ui->screen_1_lastpage_label = lv_label_create(ui->screen_1_lastpage);
    lv_label_set_text(ui->screen_1_lastpage_label, "");
    lv_label_set_long_mode(ui->screen_1_lastpage_label, LV_LABEL_LONG_WRAP);
    lv_obj_align(ui->screen_1_lastpage_label, LV_ALIGN_CENTER, 0, 0);
    lv_obj_set_style_pad_all(ui->screen_1_lastpage, 0, LV_STATE_DEFAULT);
    lv_obj_set_pos(ui->screen_1_lastpage, 263, 198);
    lv_obj_set_size(ui->screen_1_lastpage, 41, 33);
		lv_obj_add_event_cb(ui->screen_1_lastpage, next_page_1_event_cb, LV_EVENT_CLICKED, NULL); //回调事件
		
    //Write style for screen_1_lastpage, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_text_color(ui->screen_1_lastpage, lv_color_hex(0x000000), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->screen_1_lastpage, &lv_font_SourceHanSerifSC_Regular_12, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->screen_1_lastpage, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui->screen_1_lastpage, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->screen_1_lastpage, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_clip_corner(ui->screen_1_lastpage, true, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->screen_1_lastpage, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write style for screen_1_lastpage, Part: LV_PART_MAIN, State: LV_STATE_PRESSED.
    lv_obj_set_style_img_recolor_opa(ui->screen_1_lastpage, 0, LV_PART_MAIN|LV_STATE_PRESSED);
    lv_obj_set_style_img_opa(ui->screen_1_lastpage, 255, LV_PART_MAIN|LV_STATE_PRESSED);
    lv_obj_set_style_text_color(ui->screen_1_lastpage, lv_color_hex(0xFF33FF), LV_PART_MAIN|LV_STATE_PRESSED);
    lv_obj_set_style_text_font(ui->screen_1_lastpage, &lv_font_SourceHanSerifSC_Regular_12, LV_PART_MAIN|LV_STATE_PRESSED);
    lv_obj_set_style_text_opa(ui->screen_1_lastpage, 255, LV_PART_MAIN|LV_STATE_PRESSED);
    lv_obj_set_style_shadow_width(ui->screen_1_lastpage, 0, LV_PART_MAIN|LV_STATE_PRESSED);

    //Write style for screen_1_lastpage, Part: LV_PART_MAIN, State: LV_STATE_CHECKED.
    lv_obj_set_style_img_recolor_opa(ui->screen_1_lastpage, 0, LV_PART_MAIN|LV_STATE_CHECKED);
    lv_obj_set_style_img_opa(ui->screen_1_lastpage, 255, LV_PART_MAIN|LV_STATE_CHECKED);
    lv_obj_set_style_text_color(ui->screen_1_lastpage, lv_color_hex(0xFF33FF), LV_PART_MAIN|LV_STATE_CHECKED);
    lv_obj_set_style_text_font(ui->screen_1_lastpage, &lv_font_SourceHanSerifSC_Regular_12, LV_PART_MAIN|LV_STATE_CHECKED);
    lv_obj_set_style_text_opa(ui->screen_1_lastpage, 255, LV_PART_MAIN|LV_STATE_CHECKED);
    lv_obj_set_style_shadow_width(ui->screen_1_lastpage, 0, LV_PART_MAIN|LV_STATE_CHECKED);

    //Write style for screen_1_lastpage, Part: LV_PART_MAIN, State: LV_IMGBTN_STATE_RELEASED.
    lv_obj_set_style_img_recolor_opa(ui->screen_1_lastpage, 0, LV_PART_MAIN|LV_IMGBTN_STATE_RELEASED);
    lv_obj_set_style_img_opa(ui->screen_1_lastpage, 255, LV_PART_MAIN|LV_IMGBTN_STATE_RELEASED);

    //Write codes screen_1_chart_1
    ui->screen_1_chart_1 = lv_chart_create(ui->screen_1);
    lv_chart_set_type(ui->screen_1_chart_1, LV_CHART_TYPE_LINE);
    lv_chart_set_div_line_count(ui->screen_1_chart_1, 3, 5);
    lv_chart_set_point_count(ui->screen_1_chart_1, 5);
    lv_chart_set_range(ui->screen_1_chart_1, LV_CHART_AXIS_PRIMARY_Y, 0, 3500);
    lv_chart_set_axis_tick(ui->screen_1_chart_1, LV_CHART_AXIS_PRIMARY_Y, 10, 5, 5, 4, true, 40);
    lv_chart_set_range(ui->screen_1_chart_1, LV_CHART_AXIS_SECONDARY_Y, 0, 100);
    lv_chart_set_zoom_x(ui->screen_1_chart_1, 256);
    lv_chart_set_zoom_y(ui->screen_1_chart_1, 256);
    lv_obj_set_style_size(ui->screen_1_chart_1, 0, LV_PART_INDICATOR);
    ui->screen_1_chart_1_0 = lv_chart_add_series(ui->screen_1_chart_1, lv_color_hex(0x000000), LV_CHART_AXIS_PRIMARY_Y);
#if LV_USE_FREEMASTER == 0
    lv_chart_set_next_value(ui->screen_1_chart_1, ui->screen_1_chart_1_0, 1);
    lv_chart_set_next_value(ui->screen_1_chart_1, ui->screen_1_chart_1_0, 20);
    lv_chart_set_next_value(ui->screen_1_chart_1, ui->screen_1_chart_1_0, 30);
    lv_chart_set_next_value(ui->screen_1_chart_1, ui->screen_1_chart_1_0, 40);
    lv_chart_set_next_value(ui->screen_1_chart_1, ui->screen_1_chart_1_0, 5);
#endif
    lv_obj_set_pos(ui->screen_1_chart_1, 40, 5);
    lv_obj_set_size(ui->screen_1_chart_1, 275, 182);
    lv_obj_set_scrollbar_mode(ui->screen_1_chart_1, LV_SCROLLBAR_MODE_OFF);

    //Write style for screen_1_chart_1, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_bg_opa(ui->screen_1_chart_1, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui->screen_1_chart_1, lv_color_hex(0xffffff), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(ui->screen_1_chart_1, LV_GRAD_DIR_NONE, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_border_width(ui->screen_1_chart_1, 1, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_border_opa(ui->screen_1_chart_1, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_border_color(ui->screen_1_chart_1, lv_color_hex(0xe8e8e8), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_border_side(ui->screen_1_chart_1, LV_BORDER_SIDE_FULL, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->screen_1_chart_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_line_width(ui->screen_1_chart_1, 2, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_line_color(ui->screen_1_chart_1, lv_color_hex(0xe8e8e8), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_line_opa(ui->screen_1_chart_1, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->screen_1_chart_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write style for screen_1_chart_1, Part: LV_PART_TICKS, State: LV_STATE_DEFAULT.
    lv_obj_set_style_text_color(ui->screen_1_chart_1, lv_color_hex(0x151212), LV_PART_TICKS|LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->screen_1_chart_1, &lv_font_SourceHanSerifSC_Regular_12, LV_PART_TICKS|LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->screen_1_chart_1, 255, LV_PART_TICKS|LV_STATE_DEFAULT);
    lv_obj_set_style_line_width(ui->screen_1_chart_1, 2, LV_PART_TICKS|LV_STATE_DEFAULT);
    lv_obj_set_style_line_color(ui->screen_1_chart_1, lv_color_hex(0xe8e8e8), LV_PART_TICKS|LV_STATE_DEFAULT);
    lv_obj_set_style_line_opa(ui->screen_1_chart_1, 255, LV_PART_TICKS|LV_STATE_DEFAULT);

    //The custom code of screen_1.


    //Update current screen layout.
    lv_obj_update_layout(ui->screen_1);

    //Init events for screen.
    events_init_screen_1(ui);
}

//换页键被点击的时候
static void next_page_1_event_cb(lv_event_t * e) {
    if (lv_event_get_code(e) == LV_EVENT_CLICKED) {
        // 换页逻辑
				nextpage = 3;//换页后为图表页面
    }
}

//更新图表的新值
void update_chart_value(int var_value) {
    // 直接更新图表的最后一个数据点
    lv_chart_set_next_value(guider_ui.screen_1_chart_1, guider_ui.screen_1_chart_1_0, var_value);

    // 刷新图表以立即显示更新
    lv_obj_invalidate(guider_ui.screen_1_chart_1);
}
