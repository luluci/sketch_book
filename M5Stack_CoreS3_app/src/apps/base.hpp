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
			if (obj_base_)
			{
				is_visible_ = true;
				lv_obj_move_foreground(obj_base_.get());
			}
		}
		void hide()
		{
			is_visible_ = false;
		}
	};
}