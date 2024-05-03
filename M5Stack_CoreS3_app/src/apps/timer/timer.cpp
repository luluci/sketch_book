#include "timer.hpp"
#include "../utility.hpp"
#include "../config.hpp"
#include "../fonts/fonts.hpp"

#include <lvgl.h>

namespace app
{

    timer::timer() : style_select_(), obj_min_(), obj_sec_(), obj_msec_(), count_min_(0), count_sec_(0), count_msec_(0), is_count_(false)
    {
    }
    bool timer::init(lv_obj_t *parent)
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
        obj_min_ = make_lv_obj_ptr(lv_roller_create, obj_base_.get());
        if (!obj_min_)
        {
            return false;
        }
        obj_sec_ = make_lv_obj_ptr(lv_roller_create, obj_base_.get());
        if (!obj_sec_)
        {
            return false;
        }
        obj_msec_ = make_lv_obj_ptr(lv_roller_create, obj_base_.get());
        if (!obj_msec_)
        {
            return false;
        }
        obj_cover_ = make_lv_obj_ptr(lv_obj_create, obj_base_.get());
        if (!obj_cover_)
        {
            return false;
        }

        // baseのstyle設定
        lv_obj_add_style(obj_base_.get(), &style_base_black, LV_PART_MAIN);

        // style
        lv_style_init(&style_select_);
        // lv_style_set_text_font(&style_select_, &hackgen_console_60);
        // lv_style_set_text_font(&style_select_, &lv_font_montserrat_48);
        lv_style_set_text_font(&style_select_, &deja_vu_sans_70);
        // lv_style_set_bg_color(&style_select_, lv_color_hex3(0xf88));
        // lv_style_set_border_width(&style_select_, 2);
        // lv_style_set_border_color(&style_select_, lv_color_hex3(0xf00));
        lv_style_init(&style_cover_);
        lv_style_set_bg_opa(&style_cover_, LV_OPA_TRANSP);
        lv_style_set_border_width(&style_cover_, 0);
        lv_style_set_width(&style_cover_, LV_PCT(100));

        // set roller option
        lv_roller_set_options(obj_min_.get(), opt_60_, LV_ROLLER_MODE_INFINITE);
        lv_roller_set_options(obj_sec_.get(), opt_60_, LV_ROLLER_MODE_INFINITE);
        lv_roller_set_options(obj_msec_.get(), opt_msec_, LV_ROLLER_MODE_INFINITE);
        //
        lv_roller_set_visible_row_count(obj_min_.get(), 2);
        lv_roller_set_visible_row_count(obj_sec_.get(), 2);
        lv_roller_set_visible_row_count(obj_msec_.get(), 2);

        lv_obj_add_style(obj_min_.get(), &style_select_, LV_PART_MAIN);
        lv_obj_add_style(obj_sec_.get(), &style_select_, LV_PART_MAIN);
        lv_obj_add_style(obj_msec_.get(), &style_select_, LV_PART_MAIN);
        // lv_obj_set_style_text_align(roller, LV_TEXT_ALIGN_LEFT, 0);
        // lv_obj_set_style_bg_color(roller, lv_color_hex3(0x0f0), 0);
        // lv_obj_set_style_bg_grad_color(roller, lv_color_hex3(0xafa), 0);
        // lv_obj_set_style_bg_grad_dir(roller, LV_GRAD_DIR_VER, 0);
        lv_obj_set_size(obj_min_.get(), obj_width_, obj_height_);
        lv_obj_set_size(obj_sec_.get(), obj_width_, obj_height_);
        lv_obj_set_size(obj_msec_.get(), obj_width_msec_, obj_height_msec_);
        lv_obj_align(obj_min_.get(), LV_ALIGN_LEFT_MID, obj_margin_, 0);
        lv_obj_align(obj_sec_.get(), LV_ALIGN_LEFT_MID, obj_margin_ + obj_width_ + obj_margin_, 0);
        lv_obj_align(obj_msec_.get(), LV_ALIGN_LEFT_MID, (obj_margin_ + obj_width_) * 2 + obj_margin_, (obj_height_ - obj_height_msec_) / 2);

        // lv_obj_add_event_cb(roller, event_handler, LV_EVENT_ALL, NULL);
        lv_roller_set_selected(obj_min_.get(), count_min_, LV_ANIM_OFF);
        lv_roller_set_selected(obj_sec_.get(), count_sec_, LV_ANIM_OFF);
        lv_roller_set_selected(obj_msec_.get(), count_msec_, LV_ANIM_OFF);

        // rollerのGUIに蓋をして無効化する
        lv_obj_set_align(obj_cover_.get(), LV_ALIGN_CENTER);
        // lv_obj_set_size(obj_cover_.get(), 250, obj_height_);
        lv_obj_add_style(obj_cover_.get(), &style_cover_, LV_PART_MAIN);

        // 画面タッチイベント
        // lv_obj_add_event_cb(obj_base_.get(), event_cb, LV_EVENT_RELEASED, this);
        lv_obj_add_event_cb(obj_cover_.get(), event_cb, LV_EVENT_RELEASED, this);

        return true;
    }
    void timer::begin()
    {
        is_count_ = true;
        reset_timer();
    }
    void timer::count()
    {
        if (!is_count_)
        {
            return;
        }

        count_msec_++;
        if (count_msec_ >= 10)
        {
            count_msec_ = 0;
            count_sec_++;
            if (count_sec_ >= 60)
            {
                count_sec_ = 0;
                count_min_++;
                if (count_min_ >= 60)
                {
                    count_min_ = 0;
                }
                lv_roller_set_selected(obj_min_.get(), count_min_, LV_ANIM_ON);
            }
            lv_roller_set_selected(obj_sec_.get(), count_sec_, LV_ANIM_ON);
        }
        lv_roller_set_selected(obj_msec_.get(), count_msec_, LV_ANIM_OFF);
    }
    void timer::reset_timer()
    {
        count_min_ = 0;
        count_sec_ = 0;
        count_msec_ = 0;
        lv_roller_set_selected(obj_min_.get(), count_min_, LV_ANIM_OFF);
        lv_roller_set_selected(obj_sec_.get(), count_sec_, LV_ANIM_OFF);
        lv_roller_set_selected(obj_msec_.get(), count_msec_, LV_ANIM_OFF);
    }

    void timer::event_cb(lv_event_t *event)
    {
        auto *self = reinterpret_cast<timer *>(event->user_data);
        self->begin();
    }
}
