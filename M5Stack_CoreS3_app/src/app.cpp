#include "app.hpp"

#include <lvgl.h>

#include "m5gfx_lvgl.hpp"
#include "apps/timer/timer.hpp"
#include "apps/app_menu/app_menu.hpp"
#include "apps/config.hpp"
#include "apps/utility.hpp"

app::lv_obj_ptr_t header_;
app::lv_obj_ptr_t body_;

app::timer timer;
app::app_menu app_menu;

static void
my_event_cb(lv_event_t *event)
{
	timer.begin();
}

static void lvgl_100ms_timer(lv_timer_t *tim)
{
	timer.count();
}

void disp_app()
{
	switch (app_menu.status())
	{
	case app::item::timer:
		timer.show();
		break;
	default:
		break;
	}
}

void gesture_event(lv_event_t *e)
{
	// lv_obj_t *screen = lv_event_get_current_target(e);
	lv_dir_t dir = lv_indev_get_gesture_dir(lv_indev_active());
	switch (dir)
	{
	case LV_DIR_LEFT:
		break;
	case LV_DIR_RIGHT:
		app_menu.show();
		break;
	case LV_DIR_TOP:
		break;
	case LV_DIR_BOTTOM:
		break;
	}
}
void menu_event(lv_event_t *e)
{
	// lv_obj_t *screen = lv_event_get_current_target(e);
	disp_app();
}

void app_init()
{
	// init LVGL
	lv_init();
	// init LVGL driver
	m5gfx_lvgl_init();

	/* Make sure the default group exists */
	if (!lv_group_get_default())
	{
		lv_group_t *group = lv_group_create();
		lv_group_set_default(group);
	}

	/* Set screen style */
	lv_obj_t *scr = lv_scr_act();
	lv_obj_remove_style_all(scr);
	lv_obj_set_style_bg_opa(scr, LV_OPA_TRANSP, 0);
	lv_obj_clear_flag(scr, LV_OBJ_FLAG_SCROLLABLE);
	lv_obj_clear_flag(scr, LV_OBJ_FLAG_GESTURE_BUBBLE);
	// lv_disp_set_bg_color(lv_disp_get_default(), lv_color_white());
	lv_obj_set_style_bg_color(scr, lv_color_hex(0x003a57), LV_PART_MAIN);

	/* Set root default style */
	static lv_style_t rootStyle;
	lv_style_init(&rootStyle);
	lv_style_set_width(&rootStyle, LV_HOR_RES);
	lv_style_set_height(&rootStyle, LV_VER_RES);
	lv_style_set_border_color(&rootStyle, lv_color_make(238, 238, 238));
	lv_style_set_border_width(&rootStyle, 0);
	lv_style_set_outline_width(&rootStyle, 0);
	lv_style_set_pad_all(&rootStyle, 0);
	lv_style_set_bg_color(&rootStyle, lv_color_hex3(0xf88));

	// base用style
	// 黒背景
	lv_style_init(&app::style_base_black);
	lv_style_set_width(&app::style_base_black, LV_HOR_RES);
	lv_style_set_height(&app::style_base_black, LV_VER_RES);
	lv_style_set_border_width(&app::style_base_black, 0);
	lv_style_set_outline_width(&app::style_base_black, 0);
	lv_style_set_pad_all(&app::style_base_black, 0);
	lv_style_set_bg_color(&app::style_base_black, lv_color_hex3(0x000));
	// 背景透過
	lv_style_init(&app::style_base_transp);
	lv_style_set_width(&app::style_base_transp, LV_HOR_RES);
	lv_style_set_height(&app::style_base_transp, LV_VER_RES);
	lv_style_set_border_width(&app::style_base_transp, 0);
	lv_style_set_outline_width(&app::style_base_transp, 0);
	lv_style_set_pad_all(&app::style_base_transp, 0);
	lv_style_set_bg_opa(&app::style_base_transp, LV_OPA_TRANSP);

	// lv_obj_t *label = lv_label_create(lv_screen_active());
	// lv_label_set_text(label, "Hello world");
	// lv_obj_set_style_text_color(lv_screen_active(), lv_color_hex(0xffffff), LV_PART_MAIN);
	// lv_obj_align(label, LV_ALIGN_CENTER, 0, 0);

	lv_obj_add_event_cb(scr, my_event_cb, LV_EVENT_RELEASED, nullptr);

	// // screenをheader/bodyで分割
	// header_ = app::make_lv_obj_ptr(lv_obj_create, scr);
	// body_ = app::make_lv_obj_ptr(lv_obj_create, scr);
	// //
	// lv_obj_add_style(header_.get(), &rootStyle, LV_PART_MAIN);
	// lv_obj_set_align(header_.get(), LV_ALIGN_TOP_MID);
	// lv_obj_set_align(body_.get(), LV_ALIGN_TOP_MID);
	// lv_obj_set_y(header_.get(), app::SCR_HEADER_Y);
	// lv_obj_set_y(body_.get(), app::SCR_BODY_Y);
	// lv_obj_set_size(header_.get(), LCD_WIDTH, app::SCR_HEADER_HEIGHT);
	// lv_obj_set_size(body_.get(), LCD_WIDTH, app::SCR_BODY_HEIGHT);

	app_menu.init(scr);
	timer.init(scr);

	//
	lv_timer_create(lvgl_100ms_timer, 100, nullptr);
	// gesture
	lv_obj_add_event_cb(scr, gesture_event, LV_EVENT_GESTURE, NULL);
	// menu
	lv_obj_add_event_cb(scr, menu_event, (lv_event_code_t)app::APP_EVENT_MENU_SELECTED, NULL);

	//
	disp_app();
}

void app_dispose()
{
}

void app_loop()
{
	auto delay_ms = lv_timer_handler();
	delay(delay_ms);
}
