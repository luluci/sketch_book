#pragma once

#include <lvgl.h>
#include "../base.hpp"
#include "../utility.hpp"
#include "../config.hpp"

namespace app
{

	class app_menu : public base
	{
		lv_obj_ptr_t obj_list_;
		//
		item state_;

		static constexpr size_t list_width = SCR_WIDTH - 90;

	public:
		app_menu();

		bool init(lv_obj_t *parent);
		item status()
		{
			return state_;
		}

		static void event_cb(lv_event_t *);
		static void event_item_timer_cb(lv_event_t *);
		static void event_item_none_cb(lv_event_t *);

	private:
		static void event_item_cb_impl(lv_event_t *, item);
	};
}