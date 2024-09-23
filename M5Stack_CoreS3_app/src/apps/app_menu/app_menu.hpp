#pragma once

#include <lvgl.h>
#include "../base.hpp"
#include "../utility.hpp"
#include "../config.hpp"

namespace app
{
	class app_menu;

	class app_menu_item
	{
		friend class app_menu;
		app_menu *menu_ptr_;
		app::item item_;

	public:
		void init(app_menu *, app::item);
		void set_cb(lv_obj_t *obj, lv_event_code_t filter);
		static void event_cb(lv_event_t *);
	};

	class app_menu : public base
	{
		lv_obj_ptr_t obj_list_;
		//
		item state_;

		static constexpr size_t list_width = SCR_WIDTH - 90;

		//
		friend class app_menu_item;
		app_menu_item dummy_;
		app_menu_item timer_;
		app_menu_item i2c_dump_;
		app_menu_item ir_recv_dump_;

	public:
		app_menu();

		bool init(lv_obj_t *parent, item init_item);
		item status()
		{
			return state_;
		}

	private:
		static void event_item_cb_impl(lv_event_t *, item);
	};

}