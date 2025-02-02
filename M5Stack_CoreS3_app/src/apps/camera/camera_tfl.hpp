#pragma once

#include <lvgl.h>
#include "../base.hpp"
#include "../utility.hpp"

namespace app
{

	class camera_tfl : public base
	{
		lv_style_t style_select_;

		//
		lv_obj_ptr_t obj_title_lbl_;

		// camera制御フラグ
		bool is_update_;
		bool is_menu_open_;

	public:
		camera_tfl();
		bool init(lv_obj_t *parent);

		void on_timer(timer_tick_100ms);

		void on_touch();

		void on_show() override;
		void on_hide() override;
		void on_menu_open() override;

		void update();

		void update_camera();
	};

}
