#include "app.hpp"

#include <lvgl.h>

#include "m5gfx_lvgl.hpp"
#include "apps/timer/timer.hpp"

app::timer timer;

static void my_event_cb(lv_event_t *event)
{
	timer.begin();
}

static void lvgl_100ms_timer(lv_timer_t *tim)
{
	timer.count();
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

	// lv_obj_t *label = lv_label_create(lv_screen_active());
	// lv_label_set_text(label, "Hello world");
	// lv_obj_set_style_text_color(lv_screen_active(), lv_color_hex(0xffffff), LV_PART_MAIN);
	// lv_obj_align(label, LV_ALIGN_CENTER, 0, 0);

	lv_obj_add_event_cb(scr, my_event_cb, LV_EVENT_RELEASED, nullptr);

	timer.init(lv_screen_active());

	//
	lv_timer_create(lvgl_100ms_timer, 100, nullptr);
}

void app_dispose()
{
}

void app_loop()
{
	auto delay_ms = lv_timer_handler();
	delay(delay_ms);
}
