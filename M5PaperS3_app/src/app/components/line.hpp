#pragma once

// clang-format off
#include <M5Unified.h>
// clang-format on

#include <lib_mini_appfx/component.hpp>

namespace app::components
{
    template <typename Id>
    class line : public lib_mini_appfx::component<Id>
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

        line(id_type id_) : base_type::component(id_) {}

        virtual void render(uint32_t data) override
        {
            // 下線を引く
            // M5.Display.drawFastHLine(x, y2, w);
            M5.Display.fillRect(x, y, w, h, TFT_BLACK);
        }
    };
}
