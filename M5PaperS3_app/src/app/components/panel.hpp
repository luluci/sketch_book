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
    class panel : public lib_mini_appfx::component<Id>
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

        panel(id_type id_) : base_type::component(id_), bkcolor(TFT_WHITE) {}

        void set_bkcolor(int color)
        {
            bkcolor = color;
        }

        virtual void render() override
        {
            if (bkcolor == TFT_TRANSPARENT)
            {
                // 背景色透明＝描画無し
                // 透明なタッチ判定を設定するために使う
            }
            else
            {
                // 描画領域を塗りつぶし
                M5.Display.fillRect(x, y, w, h, bkcolor);
                // 線を引く
                // M5.Display.drawLine(x + 2, y + 2, x2 - 4, y2 - 4);

                // M5.Display.setCursor(x + 10, y + 10);
                // M5.Display.printf("app base");
            }
        }
    };

}
