#pragma once

// clang-format off
#include <M5Unified.h>
// clang-format on

#include <lib_mini_appfx/component.hpp>

#include "../resources.hpp"

namespace app::components
{
    // panel表示ベース
    template <typename Id>
    class draw_canvas : public lib_mini_appfx::component<Id>
    {
    public:
        using base_type = lib_mini_appfx::component<Id>;
        using id_type = typename base_type::id_type;
        using base_type::h;
        using base_type::w;
        using base_type::x;
        using base_type::x2;
        using base_type::y;
        using base_type::y2;

        int bkcolor;

        draw_canvas(id_type id_) : base_type::component(id_), bkcolor(TFT_WHITE) {}

        void set_bkcolor(int color)
        {
            bkcolor = color;
        }

        virtual void render() override
        {
            clear();
        }

        void on_touch_released(int x_, int y_)
        {
            M5.Display.drawCircle(x_, y_, 2, TFT_BLACK);
        }
        void clear()
        {
            // 描画領域を塗りつぶし
            M5.Display.fillRect(x, y, w, h, bkcolor);
        }
    };

}
