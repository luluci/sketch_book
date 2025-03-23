#pragma once

// clang-format off
#include <M5Unified.h>
// clang-format on

#include <lib_mini_appfx/component.hpp>

#include "./label.hpp"
#include "../resources.hpp"

namespace app::components
{
    template <typename Id>
    class button_label : public lib_mini_appfx::component<Id>, public label_base
    {
    public:
        using base_type = lib_mini_appfx::component<Id>;
        using id_type = typename base_type::id_type;
        using base_type::h;
        using base_type::height;
        using base_type::w;
        using base_type::width;
        using base_type::x;
        using base_type::x2;
        using base_type::y;
        using base_type::y2;

        button_label(id_type id_) : base_type::component(id_), label_base() {}

        void set_coord(int x_, int y_, int w_, int h_)
        {
            base_type::set_coord(x_, y_, w_, h_);
            label_base::set_coord(x_, y_, w_, h_);
        }

        using label_base::set_bkcolor;
        using label_base::set_border_color;
        using label_base::set_font;
        using label_base::set_round;
        using label_base::set_text;
        using label_base::set_underline;

        virtual void render() override
        {
            label_base::render();
        }
    };
}
