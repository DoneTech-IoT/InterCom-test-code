/*
 * Copyright 2024 NXP
 * NXP Confidential and Proprietary. This software is owned or controlled by NXP and may only be used strictly in
 * accordance with the applicable license terms. By expressly accepting such terms or by downloading, installing,
 * activating and/or otherwise using the software, you are agreeing that you have read, and that you agree to
 * comply with and are bound by, such license terms.  If you do not agree to be bound by the applicable license
 * terms, then you may not retain, install, activate or otherwise use the software.
 */

#include "events_init.h"
#include <stdio.h>
#include "lvgl.h"
#if LV_USE_GUIDER_SIMULATOR && LV_USE_FREEMASTER
#include "freemaster_client.h"
#endif

static void screen_SmallGrindImage_event_handler(lv_event_t *e)
{
	lv_event_code_t code = lv_event_get_code(e);
	switch (code)
	{
	case LV_EVENT_CLICKED:
	{
		lv_obj_set_style_img_recolor(guider_ui.screen_SmallGrindImage, lv_color_hex(0x793f15), LV_PART_MAIN);
		break;
	}
	case LV_EVENT_PRESSED:
	{
		lv_obj_set_style_img_recolor(guider_ui.screen_SmallGrindImage, lv_color_hex(0x6d6d6d), LV_PART_MAIN);
		break;
	}
	default:
		break;
	}
}

static void screen_TeaImage_event_handler(lv_event_t *e)
{
	lv_event_code_t code = lv_event_get_code(e);
	switch (code)
	{
	case LV_EVENT_CLICKED:
	{
		lv_obj_set_style_img_recolor(guider_ui.screen_TeaImage, lv_color_hex(0x793f15), LV_PART_MAIN);
		break;
	}
	case LV_EVENT_PRESSED:
	{
		lv_obj_set_style_img_recolor(guider_ui.screen_TeaImage, lv_color_hex(0x6d6d6d), LV_PART_MAIN);
		break;
	}
	default:
		break;
	}
}

static void screen_ScopImage_event_handler(lv_event_t *e)
{
	lv_event_code_t code = lv_event_get_code(e);
	switch (code)
	{
	case LV_EVENT_CLICKED:
	{
		lv_obj_set_style_img_recolor(guider_ui.screen_ScopImage, lv_color_hex(0x793f15), LV_PART_MAIN);
		break;
	}
	case LV_EVENT_PRESSED:
	{
		lv_obj_set_style_img_recolor(guider_ui.screen_ScopImage, lv_color_hex(0x6d6d6d), LV_PART_MAIN);
		break;
	}
	default:
		break;
	}
}

static void screen_MediumGrindImage_event_handler(lv_event_t *e)
{
	lv_event_code_t code = lv_event_get_code(e);
	switch (code)
	{
	case LV_EVENT_CLICKED:
	{
		lv_obj_set_style_img_recolor(guider_ui.screen_MediumGrindImage, lv_color_hex(0x793f15), LV_PART_MAIN);
		break;
	}
	case LV_EVENT_PRESSED:
	{
		lv_obj_set_style_img_recolor(guider_ui.screen_MediumGrindImage, lv_color_hex(0x6d6d6d), LV_PART_MAIN);
		break;
	}
	default:
		break;
	}
}

static void screen_CoffeeNutImage_event_handler(lv_event_t *e)
{
	lv_event_code_t code = lv_event_get_code(e);
	switch (code)
	{
	case LV_EVENT_CLICKED:
	{
		lv_obj_set_style_img_recolor(guider_ui.screen_CoffeeNutImage, lv_color_hex(0x7e3f15), LV_PART_MAIN);
		break;
	}
	case LV_EVENT_PRESSED:
	{
		lv_obj_set_style_img_recolor(guider_ui.screen_CoffeeNutImage, lv_color_hex(0x6d6d6d), LV_PART_MAIN);
		break;
	}
	default:
		break;
	}
}

static void screen_longGrindImage_event_handler(lv_event_t *e)
{
	lv_event_code_t code = lv_event_get_code(e);
	switch (code)
	{
	case LV_EVENT_CLICKED:
	{
		lv_obj_set_style_img_recolor(guider_ui.screen_longGrindImage, lv_color_hex(0x793f15), LV_PART_MAIN);
		break;
	}
	case LV_EVENT_PRESSED:
	{
		lv_obj_set_style_img_recolor(guider_ui.screen_longGrindImage, lv_color_hex(0x6d6d6d), LV_PART_MAIN);
		break;
	}
	default:
		break;
	}
}

static void screen_Timer_event_handler(lv_event_t *e)
{
	lv_event_code_t code = lv_event_get_code(e);
	lv_obj_t *object = lv_event_get_current_target(e);
	char *inputText = (char *)lv_event_get_param(e);
	switch (code)
	{
	case LV_EVENT_VALUE_CHANGED:
	{
		lv_obj_set_style_text_font(object, &lv_font_montserratMedium_19, LV_PART_MAIN | LV_STATE_DEFAULT);
		lv_label_set_text(object, inputText);
		break;
	}
	default:
		break;
	}
}

static void screen_CountOfCup_event_handler(lv_event_t *e)
{
	lv_event_code_t code = lv_event_get_code(e);
	lv_obj_t *object = lv_event_get_current_target(e);
	char *inputText = (char *)lv_event_get_param(e);
	switch (code)
	{
	case LV_EVENT_VALUE_CHANGED:
	{
		lv_obj_set_style_text_font(object, &lv_font_montserratMedium_22, LV_PART_MAIN | LV_STATE_DEFAULT);
		lv_label_set_text(object, inputText);
		break;
	}
	default:
		break;
	}
}

void events_init_screen(lv_ui *ui)
{
	lv_obj_add_event_cb(ui->screen_SmallGrindImage, screen_SmallGrindImage_event_handler, LV_EVENT_ALL, ui);
	lv_obj_add_event_cb(ui->screen_TeaImage, screen_TeaImage_event_handler, LV_EVENT_ALL, ui);
	lv_obj_add_event_cb(ui->screen_ScopImage, screen_ScopImage_event_handler, LV_EVENT_ALL, ui);
	lv_obj_add_event_cb(ui->screen_MediumGrindImage, screen_MediumGrindImage_event_handler, LV_EVENT_ALL, ui);
	lv_obj_add_event_cb(ui->screen_CoffeeNutImage, screen_CoffeeNutImage_event_handler, LV_EVENT_ALL, ui);
	lv_obj_add_event_cb(ui->screen_longGrindImage, screen_longGrindImage_event_handler, LV_EVENT_ALL, ui);
	lv_obj_add_event_cb(ui->screen_Timer, screen_Timer_event_handler, LV_EVENT_ALL, ui);
	lv_obj_add_event_cb(ui->screen_CountOfCup, screen_CountOfCup_event_handler, LV_EVENT_ALL, ui);
}

void events_init(lv_ui *ui)
{
}
