#include "camera_160x120.hpp"

#include <lvgl.h>

#include "../utility.hpp"
#include "../config.hpp"
#include "../fonts/fonts.hpp"
#include "M5CoreS3Camera.hpp"

#include "audio/audio.hpp"

namespace app
{

	camera_160x120::camera_160x120() : style_select_(), obj_title_lbl_(), is_update_(false)
	{
	}
	bool camera_160x120::init(lv_obj_t *parent)
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
		// label
		obj_title_lbl_ = make_lv_obj_ptr(lv_label_create, obj_base_.get());
		if (!obj_title_lbl_)
		{
			return false;
		}

		// baseのstyle設定
		lv_obj_add_style(obj_base_.get(), &style_base_black, LV_PART_MAIN);

		// style
		lv_style_init(&style_select_);
		// lv_style_set_text_font(&style_select_, &hackgen_console_60);
		// lv_style_set_text_font(&style_select_, &lv_font_montserrat_48);
		// lv_style_set_text_font(&style_select_, &deja_vu_sans_70);
		lv_style_set_text_color(&style_select_, lv_color_make(255, 255, 255));
		lv_style_set_text_font(&style_select_, &lv_font_montserrat_14);
		// lv_style_set_text_color(&style_btn_off_, lv_color_make(255, 255, 255));
		//  lv_style_set_bg_color(&style_select_, lv_color_hex3(0xf88));
		//  lv_style_set_border_width(&style_select_, 2);
		//  lv_style_set_border_color(&style_select_, lv_color_hex3(0xf00));
		//  lv_style_set_width(&style_cover_, LV_PCT(100));

		// label setting
		lv_obj_add_style(obj_title_lbl_.get(), &style_select_, LV_PART_MAIN);
		lv_obj_align(obj_title_lbl_.get(), LV_ALIGN_TOP_LEFT, 0, 0);

		// 画面タッチイベント
		// lv_obj_add_event_cb(obj_base_.get(), cb_on_touch, LV_EVENT_RELEASED, this);
		add_on_touch(obj_base_.get(), this);

		update();

		return true;
	}

	void camera_160x120::on_show()
	{
		// nothing
	}
	void camera_160x120::on_hide()
	{
		is_update_ = false;
		update();
	}

	void camera_160x120::on_timer(timer_tick_100ms)
	{
		if (is_update_)
		{
			// カメラ画像更新
			update_camera();
		}
	}

	void camera_160x120::on_touch()
	{
		// タッチでカメラ画像更新有無を切り替える
		is_update_ = !is_update_;
		update();
	}

	void camera_160x120::update()
	{
		if (is_update_)
		{
			lv_label_set_text(obj_title_lbl_.get(), "Camera:Active");
		}
		else
		{
			lv_label_set_text(obj_title_lbl_.get(), "Camera:Inactive");
		}
	}

	void camera_160x120::update_camera()
	{
		if (is_update_)
		{
			if (CoreS3Camera.get())
			{
				M5.Display.startWrite();
				M5.Display.setAddrWindow(25, 90, 160, 120);
				M5.Display.writePixels((uint16_t *)CoreS3Camera.fb->buf,
									   int(CoreS3Camera.fb->len / 2));
				M5.Display.endWrite();

				// 取得したフレームを解放
				CoreS3Camera.free();
			}
		}
	}
}
