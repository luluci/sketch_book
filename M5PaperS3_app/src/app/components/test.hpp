#pragma once

// clang-format off
#include <M5Unified.h>
// clang-format on

#include <map>
#include <lib_mini_appfx/component.hpp>

#include "../resources.hpp"

namespace app::components
{
    // test用部品
    template <typename Id>
    class panel_test : public lib_mini_appfx::component<Id>
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

        panel_test(id_type id_) : base_type::component(id_), bkcolor(TFT_WHITE) {}

        void set_bkcolor(int color)
        {
            bkcolor = color;
        }

        virtual void render() override
        {
            M5.Display.setFont(resrc.font_menu.get());
            M5.Display.setTextSize(1);
            M5.Display.setTextColor(TFT_BLACK);
            M5.Display.setTextWrap(false);

            M5.Display.fillRect(x, y, w, h, TFT_WHITE);

            constexpr int offset_x = 10;
            constexpr int offset_y = 90;
            constexpr int size_w = 250;
            constexpr int size_h = 40;
            lgfx::color_conv_t cconv(lgfx::color_depth_t::grayscale_8bit);
            uint32_t gs_color;
            // lgfx::grayscale_t gs;
            lgfx::rgb565_t rgb;
            std::map<uint32_t, lgfx::rgb565_t> gs_map;
            for (uint_fast16_t color = 0; color <= 0xFFFF; color++)
            {
                rgb.set(color);
                gs_color = cconv.convert(rgb);
                gs_map.emplace(gs_color, rgb);
                // gs_map[gs_color] = rgb;
            }
            M5.Display.setCursor(offset_x, offset_y);
            M5.Display.printf("gray scale count: %d", gs_map.size());
            auto it = gs_map.begin();
            for (int row = 1; row < 17 && it != gs_map.end(); row++)
            {
                for (int col = 0; col < 2 && it != gs_map.end(); col++)
                {
                    int pos_x = offset_x + col * size_w;
                    int pos_y = offset_y + row * size_h;
                    M5.Display.fillRect(pos_x, pos_y, 50, size_h, it->second);
                    M5.Display.setCursor(pos_x + 50, pos_y);
                    M5.Display.printf("%04X(%X,%X,%X)", it->first, it->second.r5, it->second.g6, it->second.b5);
                    if (it != gs_map.end())
                        it++;
                    if (it != gs_map.end())
                        it++;
                    if (it != gs_map.end())
                        it++;
                    if (it != gs_map.end())
                        it++;
                    if (it != gs_map.end())
                        it++;
                    if (it != gs_map.end())
                        it++;
                    if (it != gs_map.end())
                        it++;
                    if (it != gs_map.end())
                        it++;
                }
            }
            // constexpr int offset_x = 10;
            // constexpr int offset_y = 90;
            // constexpr int size_w = 50;
            // constexpr int size_h = 50;
            // int color = 0;
            // for (int row = 0; row < 16; row++)
            // {
            //     for (int col = 0; col < 10; col++)
            //     {
            //         int pos_x = offset_x + col * size_w;
            //         int pos_y = offset_y + row * size_h;
            //         M5.Display.fillRect(pos_x, pos_y, size_w, size_h, color);
            //         M5.Display.setCursor(pos_x, pos_y);
            //         M5.Display.printf("%02X", color);
            //         color++;
            //     }
            // }
            // constexpr int offset_x = 10;
            // int offset_y = 90;
            // constexpr int size_w = 30;
            // constexpr int size_h = 40;
            // int color = 0xFFFF;
            // for (int row = 0; row < 1; row++)
            // {
            //     for (int col = 0; col < 16; col++)
            //     {
            //         int pos_x = offset_x + col * size_w;
            //         int pos_y = offset_y + row * size_h;
            //         M5.Display.fillRect(pos_x, pos_y, size_w, size_h, color);
            //         M5.Display.setCursor(pos_x, pos_y);
            //         M5.Display.printf("%1X", col);
            //         color >>= 1;
            //     }
            // }
            // offset_y = 140;
            // M5.Display.setColorDepth(lgfx::color_depth_t::grayscale_4bit);
            // color = 0xFFFF;
            // for (int row = 0; row < 1; row++)
            // {
            //     for (int col = 0; col < 16; col++)
            //     {
            //         int pos_x = offset_x + col * size_w;
            //         int pos_y = offset_y + row * size_h;
            //         M5.Display.fillRect(pos_x, pos_y, size_w, size_h, color);
            //         M5.Display.setCursor(pos_x, pos_y);
            //         M5.Display.printf("%1X", col);
            //         color >>= 1;
            //     }
            // }
        }
    };
}
