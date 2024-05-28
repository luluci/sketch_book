#pragma once

#include <lvgl.h>
#include "../base.hpp"
#include "../utility.hpp"
#include "../config.hpp"

namespace app
{

	class i2c_dump : public base
	{
		lv_obj_ptr_t obj_intr_dump_caption_;
		lv_obj_ptr_t obj_intr_dump_;
		lv_obj_ptr_t obj_txrx_dump_caption_;
		lv_obj_ptr_t obj_txrx_dump_;

		lv_style_t style_caption_;
		lv_style_t style_dump_;

		size_t count_;

		// GUIサイズ設定
		// I2C intr dump
		static constexpr size_t obj_margin_ = 5;
		static constexpr size_t obj_h_intr_dump_caption_ = 20;
		static constexpr size_t obj_w_intr_dump_ = SCR_WIDTH - obj_margin_ * 2;
		static constexpr size_t obj_h_intr_dump_ = 80;
		// I2C txrx dump
		static constexpr size_t obj_h_txrx_dump_caption_ = 20;
		static constexpr size_t obj_w_txrx_dump_ = SCR_WIDTH - obj_margin_ * 2;
		static constexpr size_t obj_h_txrx_dump_ = 80;

		// 配置位置計算
		// I2C intr dump
		static constexpr size_t obj_x_intr_dump_caption_ = obj_margin_;
		static constexpr size_t obj_y_intr_dump_caption_ = obj_margin_;
		static constexpr size_t obj_x_intr_dump_ = obj_margin_;
		static constexpr size_t obj_y_intr_dump_ = obj_y_intr_dump_caption_ + obj_h_intr_dump_caption_;
		// I2C txrx dump
		static constexpr size_t obj_x_txrx_dump_caption_ = obj_margin_;
		static constexpr size_t obj_y_txrx_dump_caption_ = obj_y_intr_dump_ + obj_h_intr_dump_ + obj_margin_;
		static constexpr size_t obj_x_txrx_dump_ = obj_margin_;
		static constexpr size_t obj_y_txrx_dump_ = obj_y_txrx_dump_caption_ + obj_h_txrx_dump_caption_;

		// caption label
		static constexpr char const *intr_dump_caption_ = "I2C intr event:";
		static constexpr char const *txrx_dump_caption_ = "I2C txrx event:";
		//
		static constexpr size_t intr_dump_size_ = 1024;
		char intr_dump_[intr_dump_size_];
		static constexpr size_t txrx_dump_size_ = 1024;
		char txrx_dump_[txrx_dump_size_];

	public:
		i2c_dump();
		bool init(lv_obj_t *parent, bool i2c_state);

		void begin();

		void update();

	private:
		static void event_cb(lv_event_t *);
	};
}
