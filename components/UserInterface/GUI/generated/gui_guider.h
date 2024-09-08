/*
* Copyright 2024 NXP
* NXP Confidential and Proprietary. This software is owned or controlled by NXP and may only be used strictly in
* accordance with the applicable license terms. By expressly accepting such terms or by downloading, installing,
* activating and/or otherwise using the software, you are agreeing that you have read, and that you agree to
* comply with and are bound by, such license terms.  If you do not agree to be bound by the applicable license
* terms, then you may not retain, install, activate or otherwise use the software.
*/

#ifndef GUI_GUIDER_H
#define GUI_GUIDER_H
#ifdef __cplusplus
extern "C" {
#endif

#include "lvgl.h"

typedef struct
{
  
	lv_obj_t *screen;
	bool screen_del;
	lv_obj_t *screen_BackGround1;
	lv_obj_t *screen_BackGround2;
	lv_obj_t *screen_SmallGrindImage;
	lv_obj_t *screen_TeaImage;
	lv_obj_t *screen_ScopImage;
	lv_obj_t *screen_MediumGrindImage;
	lv_obj_t *screen_CoffeeNutImage;
	lv_obj_t *screen_longGrindImage;
	lv_obj_t *screen_SidebarTop;
	lv_obj_t *screen_SidebarDown;
	lv_obj_t *screen_CupImage;
	lv_obj_t *screen_EstimatedTime_Text;
	lv_obj_t *screen_TextObject_X;
	lv_obj_t *screen_Timer;
	lv_obj_t *screen_CountOfCup;
	lv_obj_t *screen_ReadyToStart_Text;
}lv_ui;

typedef void (*ui_setup_scr_t)(lv_ui * ui);

void ui_init_style(lv_style_t * style);

void ui_load_scr_animation(lv_ui *ui, lv_obj_t ** new_scr, bool new_scr_del, bool * old_scr_del, ui_setup_scr_t setup_scr,
                           lv_scr_load_anim_t anim_type, uint32_t time, uint32_t delay, bool is_clean, bool auto_del);

void ui_animation(void * var, int32_t duration, int32_t delay, int32_t start_value, int32_t end_value, lv_anim_path_cb_t path_cb,
                       uint16_t repeat_cnt, uint32_t repeat_delay, uint32_t playback_time, uint32_t playback_delay,
                       lv_anim_exec_xcb_t exec_cb, lv_anim_start_cb_t start_cb, lv_anim_ready_cb_t ready_cb, lv_anim_deleted_cb_t deleted_cb);


void init_scr_del_flag(lv_ui *ui);

void setup_ui(lv_ui *ui);


extern lv_ui guider_ui;


void setup_scr_screen(lv_ui *ui);
LV_IMG_DECLARE(_frame_alpha_400x90);
LV_IMG_DECLARE(_frame_alpha_400x90);
LV_IMG_DECLARE(_icon_coffee_grind_lvl_3_alpha_80x80);
LV_IMG_DECLARE(_icon_tea_leaf_alpha_80x80);
LV_IMG_DECLARE(_icon_coffee_powder_alpha_80x80);
LV_IMG_DECLARE(_icon_coffee_grind_lvl_2_alpha_80x80);
LV_IMG_DECLARE(_icon_coffee_beans_alpha_80x80);
LV_IMG_DECLARE(_icon_coffee_grind_lvl_1_alpha_80x80);
LV_IMG_DECLARE(_select_your_drink_alpha_90x90);
LV_IMG_DECLARE(_select_grind_size_alpha_90x90);
LV_IMG_DECLARE(_coffee_alpha_50x50);

LV_FONT_DECLARE(lv_font_montserratMedium_16)
LV_FONT_DECLARE(lv_font_montserratMedium_22)
LV_FONT_DECLARE(lv_font_montserratMedium_19)


#ifdef __cplusplus
}
#endif
#endif
