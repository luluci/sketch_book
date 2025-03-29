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
        enum class draw_state
        {
            Release,  // 初期状態
            Pressing, // タッチされている状態
        };
        draw_state draw_state_;
        int begin_x;
        int begin_y;

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

        draw_canvas(id_type id_)
            : base_type::component(id_),
              draw_state_(draw_state::Release),
              begin_x(0),
              begin_y(0),
              bkcolor(TFT_WHITE) {}

        void set_bkcolor(int color)
        {
            bkcolor = color;
        }

        virtual void render(uint32_t data) override
        {
            clear();
        }

        void on_touch_pressed(int x_, int y_)
        {
            begin_write_line(x_, y_);
        }
        void on_touch_dragging(int x_, int y_)
        {
            if (draw_state_ == draw_state::Release)
            {
                // failsafe
                begin_write_line(x_, y_);
            }
            else if (draw_state_ == draw_state::Pressing)
            {
                write_line(x_, y_);
            }
            else
            {
                // ありえない
            }
        }
        void on_touch_released(int x_, int y_)
        {
            // M5.Display.drawCircle(x_, y_, 2, TFT_BLACK);
            if (draw_state_ == draw_state::Release)
            {
                // failsafe
                begin_write_line(x_, y_);
                write_line(x_, y_);
                end_write_line();
            }
            else if (draw_state_ == draw_state::Pressing)
            {
                write_line(x_, y_);
                end_write_line();
            }
            else
            {
                // ありえない
            }
        }

        void begin_write_line(int x_, int y_)
        {
            begin_x = x_;
            begin_y = y_;
            draw_state_ = draw_state::Pressing;
        }
        void write_line(int x_, int y_)
        {
            M5.Display.drawWideLine(begin_x, begin_y, x_, y_, 2.0, TFT_BLACK);
            begin_x = x_;
            begin_y = y_;
        }
        void end_write_line()
        {
            begin_x = 0;
            begin_y = 0;
            draw_state_ = draw_state::Release;
        }
        void clear()
        {
            // 描画領域を塗りつぶし
            M5.Display.fillRect(x, y, w, h, bkcolor);
        }
    };
}
