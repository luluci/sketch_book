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

	public:
		base() : parent_(nullptr), obj_base_() {}

		void show()
		{
			if (obj_base_)
			{
				lv_obj_move_foreground(obj_base_.get());
			}
		}
	};
}