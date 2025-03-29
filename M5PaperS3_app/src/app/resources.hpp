#pragma once

// clang-format off
#include <M5Unified.h>
// clang-format on

#include "./fonts/fonts.hpp"

namespace app
{
    struct font_info
    {
        lgfx::IFont const *font;
        lgfx::FontMetrics metrics;

        font_info(lgfx::IFont const *font_) : font(font_)
        {
            font->getDefaultMetric(&metrics);

            // M5.Display.setFont(font);
        }

        auto get() const { return font; }
    };

    struct resources
    {
        font_info font_default;
        font_info font_menu;
        font_info font_menu_jp;
        font_info font_signage;

        // grayscale color
        lgfx::rgb565_t grayscale_0;  // black
        lgfx::rgb565_t grayscale_1;  //
        lgfx::rgb565_t grayscale_2;  //
        lgfx::rgb565_t grayscale_3;  //
        lgfx::rgb565_t grayscale_4;  //
        lgfx::rgb565_t grayscale_5;  //
        lgfx::rgb565_t grayscale_6;  //
        lgfx::rgb565_t grayscale_7;  //
        lgfx::rgb565_t grayscale_8;  //
        lgfx::rgb565_t grayscale_9;  //
        lgfx::rgb565_t grayscale_10; //
        lgfx::rgb565_t grayscale_11; //
        lgfx::rgb565_t grayscale_12; //
        lgfx::rgb565_t grayscale_13; //
        lgfx::rgb565_t grayscale_14; //
        lgfx::rgb565_t grayscale_15; //
        lgfx::rgb565_t grayscale_16; // white

        resources() : font_default(&lgfx::fonts::lgfxJapanGothic_16), font_menu(&lgfx::fonts::DejaVu24), font_menu_jp(&fonts::font_UDGothic_36), font_signage(&fonts::font_UDGothic_40)
        {
            grayscale_0.set((0x00 << 11) | (0x00 << 5) | 0x00);
            grayscale_1.set((0x00 << 11) | (0x00 << 5) | 0x12);
            grayscale_2.set((0x00 << 11) | (0x02 << 5) | 0x1E);
            grayscale_3.set((0x00 << 11) | (0x08 << 5) | 0x1F);
            grayscale_4.set((0x00 << 11) | (0x0F << 5) | 0x1F);
            grayscale_5.set((0x00 << 11) | (0x16 << 5) | 0x1E);
            grayscale_6.set((0x00 << 11) | (0x1D << 5) | 0x1D);
            grayscale_7.set((0x00 << 11) | (0x23 << 5) | 0x1F);
            grayscale_8.set((0x00 << 11) | (0x2A << 5) | 0x1F);
            grayscale_9.set((0x00 << 11) | (0x31 << 5) | 0x1E);
            grayscale_10.set((0x00 << 11) | (0x38 << 5) | 0x1D);
            grayscale_11.set((0x00 << 11) | (0x3E << 5) | 0x1F);
            grayscale_12.set((0x06 << 11) | (0x3F << 5) | 0x1E);
            grayscale_13.set((0x0C << 11) | (0x3F << 5) | 0x1F);
            grayscale_14.set((0x13 << 11) | (0x3F << 5) | 0x1E);
            grayscale_15.set((0x19 << 11) | (0x3F << 5) | 0x1F);
            grayscale_16.set((0x1F << 11) | (0x3F << 5) | 0x1F);
        }

        // void get_grayscale_color(uint8_t color){
        //     lgfx::grayscale_t gs(color);
        //     gs.get();
        // }
    };
    extern resources resrc;
}
