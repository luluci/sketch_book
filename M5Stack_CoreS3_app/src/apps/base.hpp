#pragma once

#include <lvgl.h>
#include "utility.hpp"

namespace app
{

	class base
	{
	protected:
		lv_obj_t *parent_;
		lv_obj_ptr_t obj_base_;
		bool is_visible_;

	public:
		base() : parent_(nullptr), obj_base_(), is_visible_(false) {}

		void show()
		{
			// メニューでhideイベントを取れないのでチェックしない
			// && !is_visible_
			if (obj_base_)
			{
				//
				is_visible_ = true;
				lv_obj_move_foreground(obj_base_.get());
				//
				on_show();
			}
		}
		void hide()
		{
			// メニューでhideイベントを取れないのでチェックしない
			// if (is_visible_)

			//
			is_visible_ = false;
			//
			on_hide();
		}

		virtual void on_show()
		{
			// no impl
		}
		virtual void on_hide()
		{
			// no impl
		}

	protected:
		template <typename Derive>
		static void add_on_touch(lv_obj_t *obj, Derive *derive)
		{
			lv_obj_add_event_cb(obj, on_touch<Derive>, LV_EVENT_RELEASED, (void *)derive);
		}
		template <typename Derive>
		static void on_touch(lv_event_t *event)
		{
			auto *self = reinterpret_cast<Derive *>(event->user_data);
			self->on_touch();
		}
	};
}