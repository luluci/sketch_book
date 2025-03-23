#pragma once

// clang-format off
#include <M5Unified.h>
// clang-format on

#include <optional>
#include <lib_mini_appfx/component.hpp>

#include "../resources.hpp"

namespace app::components
{
    class label_base
    {
    public:
        enum class text_alignment
        {
            Center,
            Left,
        };

    private:
        font_info const *font;
        char const *text;
        bool underline;
        lgfx::rgb565_t bkcolor;
        std::optional<lgfx::rgb565_t> border_color;
        std::optional<int32_t> round;
        text_alignment text_align;

        // label表示サイズ
        int32_t x;
        int32_t y;
        int32_t w;
        int32_t h;
        int32_t x2;
        int32_t y2;

        // text表示位置
        int32_t text_x;
        int32_t text_y;
        int32_t text_offset_x;
        int32_t text_offset_y;
        int32_t text_underline_x;
        int32_t text_underline_y;
        int32_t text_underline_w;
        int32_t text_underline_h;

        static constexpr int text_underline_offset_y = 2;

    public:
        label_base()
            : font(&resrc.font_default),
              text(nullptr),
              underline(false),
              bkcolor(0xFFFF),
              border_color(),
              round(),
              text_align(text_alignment::Center),
              x(0), y(0), w(0), h(0),
              text_underline_h(1)
        {
        }

        void set_coord(int x_, int y_, int w_, int h_)
        {
            x = x_;
            y = y_;
            w = w_;
            h = h_;
            x2 = x_ + w_ - 1;
            y2 = y_ + h_ - 1;
        }

        void set_align(text_alignment align)
        {
            text_align = align;
            update();
        }
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
        void set_underline(bool flag)
        {
            underline = flag;
        }
        void set_bkcolor(lgfx::rgb565_t color)
        {
            bkcolor = color;
        }
        void set_border_color(lgfx::rgb565_t color)
        {
            border_color = color;
        }
        void set_round(int32_t r)
        {
            round = r;
        }

        void update()
        {
            if (font == nullptr || text == nullptr)
            {
                return;
            }

            M5.Display.setFont(font->get());
            auto text_w = M5.Display.textWidth(text);

            // alignment
            switch (text_align)
            {
            case text_alignment::Center:
                text_offset_x = (w - text_w) / 2;
                break;

            case text_alignment::Left:
            default:
                text_offset_x = 0;
                break;
            }
            // text_offset_y = (h - font->metrics.height - text_underline_h - (text_underline_offset_y - 1)) / 2;
            text_offset_y = (h - font->metrics.height) / 2;
            //
            text_x = x + text_offset_x;
            text_y = y + text_offset_y;
            text_underline_x = text_x;
            text_underline_y = text_y + font->metrics.height + text_underline_offset_y;
            text_underline_w = text_w;
        }

        void render()
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
            if (round)
            {
                M5.Display.fillRoundRect(x, y, w, h, *round, bkcolor);
                if (border_color)
                {
                    M5.Display.drawRoundRect(x, y, x + w - 1, y + h - 1, *round, *border_color);
                }
            }
            else
            {
                M5.Display.fillRect(x, y, w, h, bkcolor);
                if (border_color)
                {
                    M5.Display.drawRect(x, y, x + w - 1, y + h - 1, *border_color);
                }
            }

            // 下線を引く
            // M5.Display.drawFastHLine(x, y2, w);
            // M5.Display.drawLine(x, y2 - border_w, x2, y2);
            // テキスト描画
            M5.Display.setCursor(text_x, text_y);
            M5.Display.print(text);
            // テキスト下線
            if (underline)
            {
                M5.Display.drawLine(text_underline_x, text_underline_y, text_underline_x + text_underline_w, text_underline_y, TFT_BLACK);
            }
        }
    };

    template <typename Id>
    class label : public lib_mini_appfx::component<Id>, public label_base
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

        using text_alignment = label_base::text_alignment;

        label(id_type id_)
            : base_type::component(id_),
              label_base()
        {
        }

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
