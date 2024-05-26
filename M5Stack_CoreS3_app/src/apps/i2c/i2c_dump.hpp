#pragma once

#include <lvgl.h>
#include "../base.hpp"
#include "../utility.hpp"
#include "../config.hpp"

namespace app
{

	class i2c_dump : public base
	{
		lv_obj_ptr_t obj_event_dump_caption_;
		lv_obj_ptr_t obj_event_dump_;

		lv_style_t style_caption_;
		lv_style_t style_dump_;

		// GUIサイズ設定
		static constexpr size_t obj_margin_ = 5;
		static constexpr size_t obj_h_event_dump_caption_ = 20;
		static constexpr size_t obj_w_event_dump_ = SCR_WIDTH - obj_margin_ * 2;
		static constexpr size_t obj_h_event_dump_ = 150;

		// 配置位置計算
		static constexpr size_t obj_x_event_dump_caption_ = obj_margin_;
		static constexpr size_t obj_y_event_dump_caption_ = obj_margin_;
		static constexpr size_t obj_x_event_dump_ = obj_margin_;
		static constexpr size_t obj_y_event_dump_ = obj_y_event_dump_caption_ + obj_h_event_dump_caption_;

		// caption label
		static constexpr char const *event_dump_caption_ = "I2C intr event:";
		//
		static constexpr size_t event_dump_size_ = 1024;
		char event_dump_[1024];

	public:
		i2c_dump();
		bool init(lv_obj_t *parent, bool i2c_state);

		void begin();

		void update();

	private:
		static void event_cb(lv_event_t *);
	};
}
