idf_component_register(
SRCS
    
"GUI/generated/widgets_init.c"
"GUI/generated/guider_fonts/lv_font_montserratMedium_16.c"
"GUI/generated/guider_fonts/lv_font_montserratMedium_19.c"
"GUI/generated/guider_fonts/lv_font_montserratMedium_22.c"
"GUI/generated/images/_select_grind_size_alpha_90x90.c"
"GUI/generated/images/_icon_coffee_grind_lvl_3_alpha_80x80.c"
"GUI/generated/images/_icon_coffee_grind_lvl_1_alpha_80x80.c"
"GUI/generated/images/_icon_coffee_powder_alpha_80x80.c"
"GUI/generated/images/_frame_alpha_400x90.c"
"GUI/generated/images/_icon_coffee_grind_lvl_2_alpha_80x80.c"
"GUI/generated/images/_select_your_drink_alpha_90x90.c"
"GUI/generated/images/_coffee_alpha_50x50.c"
"GUI/generated/images/_icon_tea_leaf_alpha_80x80.c"
"GUI/generated/images/_icon_coffee_beans_alpha_80x80.c"
"GUI/generated/events_init.c"
"GUI/generated/setup_scr_screen.c"
"GUI/generated/gui_guider.c"
"GUI/GUI_handler/srcs/lvglTimer.c"
"GUI/GUI_handler/srcs/GUI.c"
"GUI/coffeeMaker/coffeeMaker_GUI.c"
 INCLUDE_DIRS 

"GUI/coffeeMaker"
"GUI/generated"
"GUI/generated/guider_customer_fonts"
"GUI/GUI_handler/include"
     REQUIRES lvgl__lvgl lvgl_esp32_drivers   esp_timer driver  Utilities) 

