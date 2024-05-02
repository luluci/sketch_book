#pragma once

#include <lvgl.h>
#include <memory>

namespace app
{
    struct lv_obj_deleter
    {
        void operator()(lv_obj_t *ptr) const
        {
            lv_obj_delete(ptr);
        }
    };

    using lv_obj_ptr_t = std::unique_ptr<lv_obj_t, lv_obj_deleter>;

    template <typename Func>
    lv_obj_ptr_t make_lv_obj_ptr(Func &api, lv_obj_t *parent)
    {
        return lv_obj_ptr_t(api(parent));
    }
}