/*
 * Copyright 2024 NXP
 * NXP Confidential and Proprietary. This software is owned or controlled by NXP and may only be used strictly in
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
// #include "custom.h"

void setup_scr_screen(lv_ui *ui)
{
	// Write codes screen
	ui->screen = lv_obj_create(NULL);
	lv_obj_set_size(ui->screen, 536, 240);
	lv_obj_set_scrollbar_mode(ui->screen, LV_SCROLLBAR_MODE_OFF);

	// Write style for screen, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
	lv_obj_set_style_bg_opa(ui->screen, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
	lv_obj_set_style_bg_color(ui->screen, lv_color_hex(0xffffff), LV_PART_MAIN | LV_STATE_DEFAULT);
	lv_obj_set_style_bg_grad_dir(ui->screen, LV_GRAD_DIR_NONE, LV_PART_MAIN | LV_STATE_DEFAULT);

	// Write codes screen_BackGround1
	ui->screen_BackGround1 = lv_img_create(ui->screen);
	lv_obj_add_flag(ui->screen_BackGround1, LV_OBJ_FLAG_CLICKABLE);
	lv_img_set_src(ui->screen_BackGround1, &_frame_alpha_400x90);
	lv_img_set_pivot(ui->screen_BackGround1, 50, 50);
	lv_img_set_angle(ui->screen_BackGround1, 0);
	lv_obj_set_pos(ui->screen_BackGround1, 116, 8);
	lv_obj_set_size(ui->screen_BackGround1, 400, 90);

	// Write style for screen_BackGround1, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
	lv_obj_set_style_img_recolor_opa(ui->screen_BackGround1, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
	lv_obj_set_style_img_opa(ui->screen_BackGround1, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
	lv_obj_set_style_radius(ui->screen_BackGround1, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
	lv_obj_set_style_clip_corner(ui->screen_BackGround1, true, LV_PART_MAIN | LV_STATE_DEFAULT);

	// Write codes screen_BackGround2
	ui->screen_BackGround2 = lv_img_create(ui->screen);
	lv_obj_add_flag(ui->screen_BackGround2, LV_OBJ_FLAG_CLICKABLE);
	lv_img_set_src(ui->screen_BackGround2, &_frame_alpha_400x90);
	lv_img_set_pivot(ui->screen_BackGround2, 50, 50);
	lv_img_set_angle(ui->screen_BackGround2, 0);
	lv_obj_set_pos(ui->screen_BackGround2, 116, 103);
	lv_obj_set_size(ui->screen_BackGround2, 400, 90);

	// Write style for screen_BackGround2, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
	lv_obj_set_style_img_recolor_opa(ui->screen_BackGround2, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
	lv_obj_set_style_img_opa(ui->screen_BackGround2, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
	lv_obj_set_style_radius(ui->screen_BackGround2, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
	lv_obj_set_style_clip_corner(ui->screen_BackGround2, true, LV_PART_MAIN | LV_STATE_DEFAULT);

	// Write codes screen_SmallGrindImage
	ui->screen_SmallGrindImage = lv_img_create(ui->screen);
	lv_obj_add_flag(ui->screen_SmallGrindImage, LV_OBJ_FLAG_CLICKABLE);
	lv_img_set_src(ui->screen_SmallGrindImage, &_icon_coffee_grind_lvl_3_alpha_80x80);
	lv_img_set_pivot(ui->screen_SmallGrindImage, 50, 50);
	lv_img_set_angle(ui->screen_SmallGrindImage, 0);
	lv_obj_set_pos(ui->screen_SmallGrindImage, 396, 103);
	lv_obj_set_size(ui->screen_SmallGrindImage, 80, 80);

	// Write style for screen_SmallGrindImage, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
	lv_obj_set_style_img_recolor_opa(ui->screen_SmallGrindImage, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
	lv_obj_set_style_img_recolor(ui->screen_SmallGrindImage, lv_color_hex(0x6d6d6d), LV_PART_MAIN | LV_STATE_DEFAULT);
	lv_obj_set_style_img_opa(ui->screen_SmallGrindImage, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
	lv_obj_set_style_radius(ui->screen_SmallGrindImage, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
	lv_obj_set_style_clip_corner(ui->screen_SmallGrindImage, true, LV_PART_MAIN | LV_STATE_DEFAULT);

	// Write codes screen_TeaImage
	ui->screen_TeaImage = lv_img_create(ui->screen);
	lv_obj_add_flag(ui->screen_TeaImage, LV_OBJ_FLAG_CLICKABLE);
	lv_img_set_src(ui->screen_TeaImage, &_icon_tea_leaf_alpha_80x80);
	lv_img_set_pivot(ui->screen_TeaImage, 50, 50);
	lv_img_set_angle(ui->screen_TeaImage, 0);
	lv_obj_set_pos(ui->screen_TeaImage, 400, 8);
	lv_obj_set_size(ui->screen_TeaImage, 80, 80);

	// Write style for screen_TeaImage, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
	lv_obj_set_style_img_recolor_opa(ui->screen_TeaImage, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
	lv_obj_set_style_img_recolor(ui->screen_TeaImage, lv_color_hex(0x6d6d6d), LV_PART_MAIN | LV_STATE_DEFAULT);
	lv_obj_set_style_img_opa(ui->screen_TeaImage, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
	lv_obj_set_style_radius(ui->screen_TeaImage, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
	lv_obj_set_style_clip_corner(ui->screen_TeaImage, true, LV_PART_MAIN | LV_STATE_DEFAULT);

	// Write codes screen_ScopImage
	ui->screen_ScopImage = lv_img_create(ui->screen);
	lv_obj_add_flag(ui->screen_ScopImage, LV_OBJ_FLAG_CLICKABLE);
	lv_img_set_src(ui->screen_ScopImage, &_icon_coffee_powder_alpha_80x80);
	lv_img_set_pivot(ui->screen_ScopImage, 50, 50);
	lv_img_set_angle(ui->screen_ScopImage, 0);
	lv_obj_set_pos(ui->screen_ScopImage, 273, 11);
	lv_obj_set_size(ui->screen_ScopImage, 80, 80);

	// Write style for screen_ScopImage, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
	lv_obj_set_style_img_recolor_opa(ui->screen_ScopImage, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
	lv_obj_set_style_img_recolor(ui->screen_ScopImage, lv_color_hex(0x6d6d6d), LV_PART_MAIN | LV_STATE_DEFAULT);
	lv_obj_set_style_img_opa(ui->screen_ScopImage, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
	lv_obj_set_style_radius(ui->screen_ScopImage, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
	lv_obj_set_style_clip_corner(ui->screen_ScopImage, true, LV_PART_MAIN | LV_STATE_DEFAULT);

	// Write codes screen_MediumGrindImage
	ui->screen_MediumGrindImage = lv_img_create(ui->screen);
	lv_obj_add_flag(ui->screen_MediumGrindImage, LV_OBJ_FLAG_CLICKABLE);
	lv_img_set_src(ui->screen_MediumGrindImage, &_icon_coffee_grind_lvl_2_alpha_80x80);
	lv_img_set_pivot(ui->screen_MediumGrindImage, 50, 50);
	lv_img_set_angle(ui->screen_MediumGrindImage, 0);
	lv_obj_set_pos(ui->screen_MediumGrindImage, 265, 108);
	lv_obj_set_size(ui->screen_MediumGrindImage, 80, 80);

	// Write style for screen_MediumGrindImage, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
	lv_obj_set_style_img_recolor_opa(ui->screen_MediumGrindImage, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
	lv_obj_set_style_img_recolor(ui->screen_MediumGrindImage, lv_color_hex(0x6d6d6d), LV_PART_MAIN | LV_STATE_DEFAULT);
	lv_obj_set_style_img_opa(ui->screen_MediumGrindImage, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
	lv_obj_set_style_radius(ui->screen_MediumGrindImage, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
	lv_obj_set_style_clip_corner(ui->screen_MediumGrindImage, true, LV_PART_MAIN | LV_STATE_DEFAULT);

	// Write codes screen_CoffeeNutImage
	ui->screen_CoffeeNutImage = lv_img_create(ui->screen);
	lv_obj_add_flag(ui->screen_CoffeeNutImage, LV_OBJ_FLAG_CLICKABLE);
	lv_img_set_src(ui->screen_CoffeeNutImage, &_icon_coffee_beans_alpha_80x80);
	lv_img_set_pivot(ui->screen_CoffeeNutImage, 50, 50);
	lv_img_set_angle(ui->screen_CoffeeNutImage, 0);
	lv_obj_set_pos(ui->screen_CoffeeNutImage, 146, 13);
	lv_obj_set_size(ui->screen_CoffeeNutImage, 80, 80);

	// Write style for screen_CoffeeNutImage, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
	lv_obj_set_style_img_recolor_opa(ui->screen_CoffeeNutImage, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
	lv_obj_set_style_img_recolor(ui->screen_CoffeeNutImage, lv_color_hex(0x6d6d6d), LV_PART_MAIN | LV_STATE_DEFAULT);
	lv_obj_set_style_img_opa(ui->screen_CoffeeNutImage, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
	lv_obj_set_style_radius(ui->screen_CoffeeNutImage, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
	lv_obj_set_style_clip_corner(ui->screen_CoffeeNutImage, true, LV_PART_MAIN | LV_STATE_DEFAULT);

	// Write codes screen_longGrindImage
	ui->screen_longGrindImage = lv_img_create(ui->screen);
	lv_obj_add_flag(ui->screen_longGrindImage, LV_OBJ_FLAG_CLICKABLE);
	lv_img_set_src(ui->screen_longGrindImage, &_icon_coffee_grind_lvl_1_alpha_80x80);
	lv_img_set_pivot(ui->screen_longGrindImage, 50, 50);
	lv_img_set_angle(ui->screen_longGrindImage, 0);
	lv_obj_set_pos(ui->screen_longGrindImage, 137, 108);
	lv_obj_set_size(ui->screen_longGrindImage, 80, 80);

	// Write style for screen_longGrindImage, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
	lv_obj_set_style_img_recolor_opa(ui->screen_longGrindImage, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
	lv_obj_set_style_img_recolor(ui->screen_longGrindImage, lv_color_hex(0x6d6d6d), LV_PART_MAIN | LV_STATE_DEFAULT);
	lv_obj_set_style_img_opa(ui->screen_longGrindImage, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
	lv_obj_set_style_radius(ui->screen_longGrindImage, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
	lv_obj_set_style_clip_corner(ui->screen_longGrindImage, true, LV_PART_MAIN | LV_STATE_DEFAULT);

	// Write codes screen_SidebarTop
	ui->screen_SidebarTop = lv_img_create(ui->screen);
	lv_obj_add_flag(ui->screen_SidebarTop, LV_OBJ_FLAG_CLICKABLE);
	lv_img_set_src(ui->screen_SidebarTop, &_select_your_drink_alpha_90x90);
	lv_img_set_pivot(ui->screen_SidebarTop, 50, 50);
	lv_img_set_angle(ui->screen_SidebarTop, 0);
	lv_obj_set_pos(ui->screen_SidebarTop, 14, 8);
	lv_obj_set_size(ui->screen_SidebarTop, 90, 90);

	// Write style for screen_SidebarTop, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
	lv_obj_set_style_img_recolor_opa(ui->screen_SidebarTop, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
	lv_obj_set_style_img_opa(ui->screen_SidebarTop, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
	lv_obj_set_style_radius(ui->screen_SidebarTop, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
	lv_obj_set_style_clip_corner(ui->screen_SidebarTop, true, LV_PART_MAIN | LV_STATE_DEFAULT);

	// Write codes screen_SidebarDown
	ui->screen_SidebarDown = lv_img_create(ui->screen);
	lv_obj_add_flag(ui->screen_SidebarDown, LV_OBJ_FLAG_CLICKABLE);
	lv_img_set_src(ui->screen_SidebarDown, &_select_grind_size_alpha_90x90);
	lv_img_set_pivot(ui->screen_SidebarDown, 50, 50);
	lv_img_set_angle(ui->screen_SidebarDown, 0);
	lv_obj_set_pos(ui->screen_SidebarDown, 14, 105);
	lv_obj_set_size(ui->screen_SidebarDown, 90, 90);

	// Write style for screen_SidebarDown, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
	lv_obj_set_style_img_recolor_opa(ui->screen_SidebarDown, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
	lv_obj_set_style_img_opa(ui->screen_SidebarDown, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
	lv_obj_set_style_radius(ui->screen_SidebarDown, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
	lv_obj_set_style_clip_corner(ui->screen_SidebarDown, true, LV_PART_MAIN | LV_STATE_DEFAULT);

	// Write codes screen_CupImage
	ui->screen_CupImage = lv_img_create(ui->screen);
	lv_obj_add_flag(ui->screen_CupImage, LV_OBJ_FLAG_CLICKABLE);
	lv_img_set_src(ui->screen_CupImage, &_coffee_alpha_50x50);
	lv_img_set_pivot(ui->screen_CupImage, 50, 50);
	lv_img_set_angle(ui->screen_CupImage, 0);
	lv_obj_set_pos(ui->screen_CupImage, 457, 188);
	lv_obj_set_size(ui->screen_CupImage, 50, 50);

	// Write style for screen_CupImage, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
	lv_obj_set_style_img_recolor_opa(ui->screen_CupImage, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
	lv_obj_set_style_img_opa(ui->screen_CupImage, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
	lv_obj_set_style_radius(ui->screen_CupImage, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
	lv_obj_set_style_clip_corner(ui->screen_CupImage, true, LV_PART_MAIN | LV_STATE_DEFAULT);

	// Write codes screen_EstimatedTime_Text
	ui->screen_EstimatedTime_Text = lv_label_create(ui->screen);
	lv_label_set_text(ui->screen_EstimatedTime_Text, "- estimated time: ");
	lv_label_set_long_mode(ui->screen_EstimatedTime_Text, LV_LABEL_LONG_WRAP);
	lv_obj_set_pos(ui->screen_EstimatedTime_Text, 143, 209);
	lv_obj_set_size(ui->screen_EstimatedTime_Text, 204, 21);

	// Write style for screen_EstimatedTime_Text, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
	lv_obj_set_style_border_width(ui->screen_EstimatedTime_Text, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
	lv_obj_set_style_radius(ui->screen_EstimatedTime_Text, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
	lv_obj_set_style_text_color(ui->screen_EstimatedTime_Text, lv_color_hex(0x000000), LV_PART_MAIN | LV_STATE_DEFAULT);
	lv_obj_set_style_text_font(ui->screen_EstimatedTime_Text, &lv_font_montserratMedium_16, LV_PART_MAIN | LV_STATE_DEFAULT);
	lv_obj_set_style_text_opa(ui->screen_EstimatedTime_Text, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
	lv_obj_set_style_text_letter_space(ui->screen_EstimatedTime_Text, 2, LV_PART_MAIN | LV_STATE_DEFAULT);
	lv_obj_set_style_text_line_space(ui->screen_EstimatedTime_Text, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
	lv_obj_set_style_text_align(ui->screen_EstimatedTime_Text, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
	lv_obj_set_style_bg_opa(ui->screen_EstimatedTime_Text, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
	lv_obj_set_style_pad_top(ui->screen_EstimatedTime_Text, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
	lv_obj_set_style_pad_right(ui->screen_EstimatedTime_Text, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
	lv_obj_set_style_pad_bottom(ui->screen_EstimatedTime_Text, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
	lv_obj_set_style_pad_left(ui->screen_EstimatedTime_Text, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_width(ui->screen_EstimatedTime_Text, 0, LV_PART_MAIN | LV_STATE_DEFAULT);

	// Write codes screen_TextObject_X
	ui->screen_TextObject_X = lv_label_create(ui->screen);
	lv_label_set_text(ui->screen_TextObject_X, "X");
	lv_label_set_long_mode(ui->screen_TextObject_X, LV_LABEL_LONG_WRAP);
	lv_obj_set_pos(ui->screen_TextObject_X, 424, 209);
	lv_obj_set_size(ui->screen_TextObject_X, 42, 27);

	// Write style for screen_TextObject_X, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
	lv_obj_set_style_border_width(ui->screen_TextObject_X, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
	lv_obj_set_style_radius(ui->screen_TextObject_X, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
	lv_obj_set_style_text_color(ui->screen_TextObject_X, lv_color_hex(0x000000), LV_PART_MAIN | LV_STATE_DEFAULT);
	lv_obj_set_style_text_font(ui->screen_TextObject_X, &lv_font_montserratMedium_22, LV_PART_MAIN | LV_STATE_DEFAULT);
	lv_obj_set_style_text_opa(ui->screen_TextObject_X, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
	lv_obj_set_style_text_letter_space(ui->screen_TextObject_X, 2, LV_PART_MAIN | LV_STATE_DEFAULT);
	lv_obj_set_style_text_line_space(ui->screen_TextObject_X, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
	lv_obj_set_style_text_align(ui->screen_TextObject_X, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
	lv_obj_set_style_bg_opa(ui->screen_TextObject_X, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
	lv_obj_set_style_pad_top(ui->screen_TextObject_X, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
	lv_obj_set_style_pad_right(ui->screen_TextObject_X, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
	lv_obj_set_style_pad_bottom(ui->screen_TextObject_X, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
	lv_obj_set_style_pad_left(ui->screen_TextObject_X, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_width(ui->screen_TextObject_X, 0, LV_PART_MAIN | LV_STATE_DEFAULT);

	// Write codes screen_Timer
	ui->screen_Timer = lv_label_create(ui->screen);
	lv_label_set_text(ui->screen_Timer, "00:00");
	lv_label_set_long_mode(ui->screen_Timer, LV_LABEL_LONG_WRAP);
	lv_obj_set_pos(ui->screen_Timer, 318, 209);
	lv_obj_set_size(ui->screen_Timer, 100, 32);

	// Write style for screen_Timer, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
	lv_obj_set_style_border_width(ui->screen_Timer, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
	lv_obj_set_style_radius(ui->screen_Timer, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
	lv_obj_set_style_text_color(ui->screen_Timer, lv_color_hex(0x000000), LV_PART_MAIN | LV_STATE_DEFAULT);
	lv_obj_set_style_text_font(ui->screen_Timer, &lv_font_montserratMedium_19, LV_PART_MAIN | LV_STATE_DEFAULT);
	lv_obj_set_style_text_opa(ui->screen_Timer, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
	lv_obj_set_style_text_letter_space(ui->screen_Timer, 2, LV_PART_MAIN | LV_STATE_DEFAULT);
	lv_obj_set_style_text_line_space(ui->screen_Timer, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
	lv_obj_set_style_text_align(ui->screen_Timer, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
	lv_obj_set_style_bg_opa(ui->screen_Timer, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
	lv_obj_set_style_pad_top(ui->screen_Timer, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
	lv_obj_set_style_pad_right(ui->screen_Timer, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
	lv_obj_set_style_pad_bottom(ui->screen_Timer, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
	lv_obj_set_style_pad_left(ui->screen_Timer, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_width(ui->screen_Timer, 0, LV_PART_MAIN | LV_STATE_DEFAULT);

	// Write codes screen_CountOfCup
	ui->screen_CountOfCup = lv_label_create(ui->screen);
	lv_label_set_text(ui->screen_CountOfCup, "0");
	lv_label_set_long_mode(ui->screen_CountOfCup, LV_LABEL_LONG_WRAP);
	lv_obj_set_pos(ui->screen_CountOfCup, 416, 209);
	lv_obj_set_size(ui->screen_CountOfCup, 23, 25);

	// Write style for screen_CountOfCup, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
	lv_obj_set_style_border_width(ui->screen_CountOfCup, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
	lv_obj_set_style_radius(ui->screen_CountOfCup, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
	lv_obj_set_style_text_color(ui->screen_CountOfCup, lv_color_hex(0x000000), LV_PART_MAIN | LV_STATE_DEFAULT);
	lv_obj_set_style_text_font(ui->screen_CountOfCup, &lv_font_montserratMedium_22, LV_PART_MAIN | LV_STATE_DEFAULT);
	lv_obj_set_style_text_opa(ui->screen_CountOfCup, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
	lv_obj_set_style_text_letter_space(ui->screen_CountOfCup, 2, LV_PART_MAIN | LV_STATE_DEFAULT);
	lv_obj_set_style_text_line_space(ui->screen_CountOfCup, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
	lv_obj_set_style_text_align(ui->screen_CountOfCup, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
	lv_obj_set_style_bg_opa(ui->screen_CountOfCup, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
	lv_obj_set_style_pad_top(ui->screen_CountOfCup, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
	lv_obj_set_style_pad_right(ui->screen_CountOfCup, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
	lv_obj_set_style_pad_bottom(ui->screen_CountOfCup, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
	lv_obj_set_style_pad_left(ui->screen_CountOfCup, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_width(ui->screen_CountOfCup, 0, LV_PART_MAIN | LV_STATE_DEFAULT);

	// Write codes screen_ReadyToStart_Text
	ui->screen_ReadyToStart_Text = lv_label_create(ui->screen);
	lv_label_set_text(ui->screen_ReadyToStart_Text, "Ready to start ");
	lv_label_set_long_mode(ui->screen_ReadyToStart_Text, LV_LABEL_LONG_WRAP);
	lv_obj_set_pos(ui->screen_ReadyToStart_Text, 1, 209);
	lv_obj_set_size(ui->screen_ReadyToStart_Text, 160, 22);

	// Write style for screen_ReadyToStart_Text, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
	lv_obj_set_style_border_width(ui->screen_ReadyToStart_Text, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
	lv_obj_set_style_radius(ui->screen_ReadyToStart_Text, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
	lv_obj_set_style_text_color(ui->screen_ReadyToStart_Text, lv_color_hex(0x000000), LV_PART_MAIN | LV_STATE_DEFAULT);
	lv_obj_set_style_text_font(ui->screen_ReadyToStart_Text, &lv_font_montserratMedium_16, LV_PART_MAIN | LV_STATE_DEFAULT);
	lv_obj_set_style_text_opa(ui->screen_ReadyToStart_Text, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
	lv_obj_set_style_text_letter_space(ui->screen_ReadyToStart_Text, 2, LV_PART_MAIN | LV_STATE_DEFAULT);
	lv_obj_set_style_text_line_space(ui->screen_ReadyToStart_Text, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
	lv_obj_set_style_text_align(ui->screen_ReadyToStart_Text, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
	lv_obj_set_style_bg_opa(ui->screen_ReadyToStart_Text, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
	lv_obj_set_style_pad_top(ui->screen_ReadyToStart_Text, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
	lv_obj_set_style_pad_right(ui->screen_ReadyToStart_Text, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
	lv_obj_set_style_pad_bottom(ui->screen_ReadyToStart_Text, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
	lv_obj_set_style_pad_left(ui->screen_ReadyToStart_Text, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_width(ui->screen_ReadyToStart_Text, 0, LV_PART_MAIN | LV_STATE_DEFAULT);

	// The custom code of screen.

	// Update current screen layout.
	lv_obj_update_layout(ui->screen);

	// Init events for screen.
	events_init_screen(ui);
}
