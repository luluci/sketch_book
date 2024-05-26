#include "i2c_dump.hpp"
#include "../utility.hpp"
#include "../config.hpp"
#include "../fonts/fonts.hpp"

#include "audio/audio.hpp"

#include <cstdio>

extern "C"
{
#include "i2c/protocol.h"
}

#include <lvgl.h>

namespace app
{

	i2c_dump::i2c_dump() : obj_event_dump_caption_(), obj_event_dump_()
	{
	}
	bool i2c_dump::init(lv_obj_t *parent, bool i2c_state)
	{
		//
		parent_ = parent;

		// 必要なリソースを最初に確保できるかチェックする
		// baseになるobjを作成してその上にGUI部品を配置する
		obj_base_ = make_lv_obj_ptr(lv_obj_create, parent_);
		if (!obj_base_)
		{
			return false;
		}
		// make roller object
		obj_event_dump_caption_ = make_lv_obj_ptr(lv_label_create, obj_base_.get());
		if (!obj_event_dump_caption_)
		{
			return false;
		}
		obj_event_dump_ = make_lv_obj_ptr(lv_label_create, obj_base_.get());
		if (!obj_event_dump_)
		{
			return false;
		}

		// baseのstyle設定
		lv_obj_add_style(obj_base_.get(), &style_base_black, LV_PART_MAIN);

		// style
		lv_style_init(&style_caption_);
		lv_style_set_text_color(&style_caption_, lv_color_make(255, 255, 255));
		lv_style_set_text_font(&style_caption_, &lv_font_montserrat_14);
		lv_style_set_border_width(&style_caption_, 0);
		// lv_style_set_bg_color(&style_select_, lv_color_hex3(0xf88));
		// lv_style_set_border_width(&style_select_, 2);
		// lv_style_set_border_color(&style_select_, lv_color_hex3(0xf00));
		lv_style_init(&style_dump_);
		lv_style_set_text_color(&style_dump_, lv_color_make(255, 255, 255));
		lv_style_set_bg_color(&style_dump_, lv_color_hex3(0x666));
		// lv_style_set_width(&style_cover_, LV_PCT(100));

		// set option
		// caption
		lv_label_set_long_mode(obj_event_dump_caption_.get(), LV_LABEL_LONG_CLIP);
		lv_label_set_text_static(obj_event_dump_caption_.get(), event_dump_caption_);
		lv_obj_align(obj_event_dump_caption_.get(), LV_ALIGN_TOP_LEFT, obj_x_event_dump_caption_, obj_y_event_dump_caption_);
		lv_obj_set_size(obj_event_dump_caption_.get(), obj_w_event_dump_, obj_h_event_dump_caption_);
		lv_obj_add_style(obj_event_dump_caption_.get(), &style_caption_, LV_PART_MAIN);
		// dump area
		lv_label_set_long_mode(obj_event_dump_.get(), LV_LABEL_LONG_WRAP);
		if (i2c_state)
		{
			snprintf(event_dump_, event_dump_size_, "<I2C dump area>");
		}
		else
		{
			snprintf(event_dump_, event_dump_size_, "<I2C init failed!>");
		}
		lv_label_set_text_static(obj_event_dump_.get(), event_dump_);
		lv_obj_align(obj_event_dump_.get(), LV_ALIGN_TOP_LEFT, obj_x_event_dump_, obj_y_event_dump_);
		lv_obj_set_size(obj_event_dump_.get(), obj_w_event_dump_, obj_h_event_dump_);
		lv_obj_add_style(obj_event_dump_.get(), &style_dump_, LV_PART_MAIN);

		return true;
	}
	void i2c_dump::begin()
	{
	}

	void i2c_dump::update()
	{
		if (i2c_protocol1_event.has_update == 1)
		{
			// clear
			i2c_protocol1_event.has_update = 0;
			//
			auto data = dbg_data;
			dbg_data.seq_dump_pos = 0;
			//
			int event_dump_pos_ = 0;
			int len;
			for (size_t i = 0; i < data.seq_dump_pos; i++)
			{
				len = snprintf(&event_dump_[event_dump_pos_], event_dump_size_ - event_dump_pos_, "> %d ", data.seq_dump[i]);
				event_dump_pos_ += len;
			}
			lv_label_set_text_static(obj_event_dump_.get(), event_dump_);
		}
	}

	void i2c_dump::event_cb(lv_event_t *event)
	{
		auto *self = reinterpret_cast<i2c_dump *>(event->user_data);
		self->begin();
	}

}
