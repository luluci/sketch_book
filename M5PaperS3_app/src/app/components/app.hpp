#pragma once

// clang-format off
#include <M5Unified.h>
// clang-format on

#include <lib_mini_appfx/component.hpp>

namespace app::components
{
    // popup表示ベース
    template <typename Id>
    class app : public lib_mini_appfx::component<Id>
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

        app(id_type id_) : base_type::component(id_) {}

        virtual void render() override
        {
            // 描画領域を塗りつぶし
            M5.Display.fillRect(x, y, w, h, TFT_WHITE);
            // 線を引く
            // M5.Display.drawLine(x + 2, y + 2, x2 - 4, y2 - 4);

            M5.Display.setCursor(x + 10, y + 10);
            M5.Display.print("app base");
        }
    };
}
