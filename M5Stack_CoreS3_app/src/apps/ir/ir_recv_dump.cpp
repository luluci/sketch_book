#include "ir_recv_dump.hpp"
#include "../utility.hpp"
#include "../config.hpp"
#include "ir_recv.hpp"

#include <cstdio>

#include <lvgl.h>

namespace app
{

    ir_recv_dump::ir_recv_dump() : obj_intr_dump_caption_(), obj_intr_dump_(), count_(0)
    {
    }
    bool ir_recv_dump::init(lv_obj_t *parent)
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
        obj_intr_dump_caption_ = make_lv_obj_ptr(lv_label_create, obj_base_.get());
        if (!obj_intr_dump_caption_)
        {
            return false;
        }
        obj_intr_dump_ = make_lv_obj_ptr(lv_label_create, obj_base_.get());
        if (!obj_intr_dump_)
        {
            return false;
        }
        obj_txrx_dump_caption_ = make_lv_obj_ptr(lv_label_create, obj_base_.get());
        if (!obj_txrx_dump_caption_)
        {
            return false;
        }
        obj_txrx_dump_ = make_lv_obj_ptr(lv_label_create, obj_base_.get());
        if (!obj_txrx_dump_)
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
        lv_style_set_bg_color(&style_dump_, lv_color_hex3(0x444));
        lv_style_set_bg_opa(&style_dump_, LV_OPA_COVER);
        // lv_style_set_width(&style_cover_, LV_PCT(100));

        // set option
        // intr dump
        // caption
        lv_label_set_long_mode(obj_intr_dump_caption_.get(), LV_LABEL_LONG_CLIP);
        lv_label_set_text_static(obj_intr_dump_caption_.get(), intr_dump_caption_);
        lv_obj_align(obj_intr_dump_caption_.get(), LV_ALIGN_TOP_LEFT, obj_x_intr_dump_caption_, obj_y_intr_dump_caption_);
        lv_obj_set_size(obj_intr_dump_caption_.get(), obj_w_intr_dump_, obj_h_intr_dump_caption_);
        lv_obj_add_style(obj_intr_dump_caption_.get(), &style_caption_, LV_PART_MAIN);
        // dump area
        lv_label_set_long_mode(obj_intr_dump_.get(), LV_LABEL_LONG_WRAP);
        snprintf(intr_dump_, intr_dump_size_, "<IR Recv dump area>");
        lv_label_set_text_static(obj_intr_dump_.get(), intr_dump_);
        lv_obj_align(obj_intr_dump_.get(), LV_ALIGN_TOP_LEFT, obj_x_intr_dump_, obj_y_intr_dump_);
        lv_obj_set_size(obj_intr_dump_.get(), obj_w_intr_dump_, obj_h_intr_dump_);
        lv_obj_add_style(obj_intr_dump_.get(), &style_dump_, LV_PART_MAIN);
        // txrx dump
        // caption
        lv_label_set_long_mode(obj_txrx_dump_caption_.get(), LV_LABEL_LONG_CLIP);
        lv_label_set_text_static(obj_txrx_dump_caption_.get(), txrx_dump_caption_);
        lv_obj_align(obj_txrx_dump_caption_.get(), LV_ALIGN_TOP_LEFT, obj_x_txrx_dump_caption_, obj_y_txrx_dump_caption_);
        lv_obj_set_size(obj_txrx_dump_caption_.get(), obj_w_txrx_dump_, obj_h_txrx_dump_caption_);
        lv_obj_add_style(obj_txrx_dump_caption_.get(), &style_caption_, LV_PART_MAIN);
        // dump area
        lv_label_set_long_mode(obj_txrx_dump_.get(), LV_LABEL_LONG_WRAP);
        snprintf(txrx_dump_, txrx_dump_size_, "<IR Recv dump area>");
        lv_label_set_text_static(obj_txrx_dump_.get(), txrx_dump_);
        lv_obj_align(obj_txrx_dump_.get(), LV_ALIGN_TOP_LEFT, obj_x_txrx_dump_, obj_y_txrx_dump_);
        lv_obj_set_size(obj_txrx_dump_.get(), obj_w_txrx_dump_, obj_h_txrx_dump_);
        lv_obj_add_style(obj_txrx_dump_.get(), &style_dump_, LV_PART_MAIN);

        // event
        lv_obj_add_event_cb(obj_base_.get(), event_cb, LV_EVENT_RELEASED, this);

        return true;
    }
    void ir_recv_dump::begin()
    {
    }

    void ir_recv_dump::update()
    {
        if (!is_visible_)
        {
            return;
        }

        //
        auto ir_recv_result = ir_recv_check();
        if (!ir_recv_result)
        {
            return;
        }

        //
        size_t idx = ir_recv_pool_pos;
        if (idx == 0)
        {
            idx = ir_recv_pool_size - 1;
        }
        else
        {
            idx--;
        }
        auto &tgt = ir_recv_pool[idx];

        //
        int len;
        int event_dump_pos_ = 0;
        len = snprintf(&intr_dump_[event_dump_pos_], intr_dump_size_ - event_dump_pos_, tgt.c_str());
        lv_label_set_text_static(obj_intr_dump_.get(), intr_dump_);
    }

    void ir_recv_dump::event_cb(lv_event_t *event)
    {
        auto *self = reinterpret_cast<ir_recv_dump *>(event->user_data);
        // self->begin();
        snprintf(self->intr_dump_, self->intr_dump_size_, "<clear> (pool_pos:%d)", ir_recv_pool_pos);
        lv_label_set_text_static(self->obj_intr_dump_.get(), self->intr_dump_);
    }

}
