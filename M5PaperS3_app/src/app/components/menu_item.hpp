#pragma once

// clang-format off
#include <M5Unified.h>
// clang-format on

#include <lib_mini_appfx/component.hpp>

#include "../resources.hpp"

namespace app::components
{
    template <typename Id>
    class menu_item : public lib_mini_appfx::component<Id>
    {
        font_info const *font;
        char const *text;

        int text_offset_x;
        int text_offset_y;
        int text_x;
        int text_y;
        int underline_x;
        int underline_y;
        int underline_w;

        static constexpr int border_w = 1;
        static constexpr int underline_offset_y = 2;

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

        menu_item(id_type id_) : base_type::component(id_), font(nullptr), text(nullptr) {}

        void set_text(char const *text_)
        {
            text = text_;
            update();
        }
        void set_font(font_info const &font_)
        {
            font = &font_;
            update();
        }
        void update()
        {
            if (font == nullptr || text == nullptr)
            {
                return;
            }

            M5.Display.setFont(font->get());
            auto text_w = M5.Display.textWidth(text);
            // text_offset_x = (width() - text_w) / 2;
            text_offset_x = 50;
            text_offset_y = (height() - font->metrics.height - border_w - 1) / 2;
            //
            text_x = x + text_offset_x;
            text_y = y + text_offset_y;
            underline_x = text_x;
            underline_y = text_y + font->metrics.height + underline_offset_y;
            underline_w = text_w;
        }

        virtual void render(uint32_t data) override
        {
            if (font == nullptr || text == nullptr)
            {
                return;
            }

            // startWrite()/endWrite()は上位で実行する
            // M5.Display.startWrite();
            M5.Display.setFont(font->get());
            M5.Display.setTextSize(1);
            M5.Display.setTextColor(TFT_BLACK);
            M5.Display.setTextWrap(false);
            // 下線を引く
            // M5.Display.drawFastHLine(x, y2, w);
            M5.Display.drawLine(x, y2, x2, y2);
            // テキスト描画
            M5.Display.setCursor(x + text_offset_x, y + text_offset_y);
            M5.Display.print(text);
            // テキスト下線
            M5.Display.drawLine(underline_x, underline_y, underline_x + underline_w, underline_y, TFT_BLACK);
        }
    };
}
