#pragma once

// clang-format off
#include <M5Unified.h>
// clang-format on

#include <optional>
#include <lib_mini_appfx/component.hpp>

#include "../resources.hpp"

namespace app::components
{
    template <typename Id>
    class button : public lib_mini_appfx::component<Id>
    {
        font_info const *font;
        char const *text;
        lgfx::rgb565_t bkcolor;
        std::optional<lgfx::rgb565_t> border_color;

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

        button(id_type id_) : base_type::component(id_), font(nullptr), text(nullptr), bkcolor(0xFFFF), border_color() {}

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
        void set_bkcolor(lgfx::rgb565_t color)
        {
            bkcolor = color;
        }
        void set_border_color(lgfx::rgb565_t color)
        {
            border_color = color;
        }

        void update()
        {
            if (font == nullptr || text == nullptr)
            {
                return;
            }

            M5.Display.setFont(font->get());
            auto text_w = M5.Display.textWidth(text);
            text_offset_x = (width() - text_w) / 2;
            text_offset_y = (height() - font->metrics.height - border_w - 1) / 2;
            //
            text_x = x + text_offset_x;
            text_y = y + text_offset_y;
            underline_x = text_x;
            underline_y = text_y + font->metrics.height + underline_offset_y;
            underline_w = text_w;
        }

        virtual void render() override
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

            // 描画領域を塗りつぶし
            M5.Display.fillRect(x, y, w, h, bkcolor);
            if (border_color)
            {
                M5.Display.drawRect(x, y, x2, y2, *border_color);
            }

            // 下線を引く
            // M5.Display.drawFastHLine(x, y2, w);
            // M5.Display.drawLine(x, y2 - border_w, x2, y2);
            // テキスト描画
            M5.Display.setCursor(x + text_offset_x, y + text_offset_y);
            M5.Display.print(text);
            // テキスト下線
            M5.Display.drawLine(underline_x, underline_y, underline_x + underline_w, underline_y, TFT_BLACK);
        }
    };
}
