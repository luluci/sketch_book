#include "app_menu.hpp"

#include "../config.hpp"

namespace app
{

	app_menu::app_menu() : state_(item::timer)
	{
	}

	bool app_menu::init(lv_obj_t *parent, item init_item)
	{
		parent_ = parent;
		state_ = init_item;

		// 必要なリソースを最初に確保できるかチェックする
		// baseになるobjを作成してその上にGUI部品を配置する
		obj_base_ = make_lv_obj_ptr(lv_obj_create, parent_);
		if (!obj_base_)
		{
			return false;
		}
		obj_list_ = make_lv_obj_ptr(lv_list_create, obj_base_.get());
		if (!obj_list_)
		{
			return false;
		}

		// baseのstyle設定
		lv_obj_add_style(obj_base_.get(), &style_base_transp, LV_PART_MAIN);

		// list設定
		lv_obj_align(obj_list_.get(), LV_ALIGN_LEFT_MID, 0, 0);
		lv_obj_set_size(obj_list_.get(), list_width, LCD_HEIGHT);
		//
		dummy_.init(this, item::MAX);
		timer_.init(this, item::timer);
		i2c_dump_.init(this, item::i2c_dump);
		lv_obj_t *btn;
		// list項目
		lv_list_add_text(obj_list_.get(), "Apps");
		btn = lv_list_add_button(obj_list_.get(), LV_SYMBOL_BELL, "Timer");
		timer_.set_cb(btn, LV_EVENT_CLICKED);
		btn = lv_list_add_button(obj_list_.get(), nullptr, "I2C dump");
		i2c_dump_.set_cb(btn, LV_EVENT_CLICKED);
		//
		lv_list_add_text(obj_list_.get(), "Debug");
		btn = lv_list_add_button(obj_list_.get(), nullptr, "none");
		dummy_.set_cb(btn, LV_EVENT_CLICKED);
		btn = lv_list_add_button(obj_list_.get(), nullptr, "none");
		dummy_.set_cb(btn, LV_EVENT_CLICKED);
		btn = lv_list_add_button(obj_list_.get(), nullptr, "none");
		dummy_.set_cb(btn, LV_EVENT_CLICKED);
		btn = lv_list_add_button(obj_list_.get(), nullptr, "none");
		dummy_.set_cb(btn, LV_EVENT_CLICKED);
		btn = lv_list_add_button(obj_list_.get(), nullptr, "none");
		dummy_.set_cb(btn, LV_EVENT_CLICKED);
		btn = lv_list_add_button(obj_list_.get(), nullptr, "none");
		dummy_.set_cb(btn, LV_EVENT_CLICKED);

		// 画面タッチイベント
		dummy_.set_cb(obj_base_.get(), LV_EVENT_RELEASED);

		return true;
	}

	void app_menu::event_item_cb_impl(lv_event_t *event, item value)
	{
		auto *self = reinterpret_cast<app_menu_item *>(event->user_data);

		if (value != item::MAX)
		{
			self->menu_ptr_->state_ = value;
		}

		lv_obj_send_event(self->menu_ptr_->parent_, (lv_event_code_t)APP_EVENT_MENU_SELECTED, nullptr);
	}

	void app_menu_item::init(app_menu *ptr, app::item itm)
	{
		menu_ptr_ = ptr;
		item_ = itm;
	}
	void app_menu_item::set_cb(lv_obj_t *obj, lv_event_code_t filter)
	{
		lv_obj_add_event_cb(obj, event_cb, filter, this);
	}
	void app_menu_item::event_cb(lv_event_t *event)
	{
		auto *self = reinterpret_cast<app_menu_item *>(event->user_data);
		self->menu_ptr_->event_item_cb_impl(event, self->item_);
	}
}
