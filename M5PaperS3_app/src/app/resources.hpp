#pragma once

// clang-format off
#include <M5Unified.h>
// clang-format on

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

        resources() : font_default(&fonts::lgfxJapanGothic_16), font_menu(&fonts::DejaVu24)
        {
            grayscale_0.set(0x00, 0x00, 0x00);
            grayscale_1.set(0x00, 0x00, 0x12);
            grayscale_2.set(0x00, 0x02, 0x1E);
            grayscale_3.set(0x00, 0x08, 0x1F);
            grayscale_4.set(0x00, 0x0F, 0x1F);
            grayscale_5.set(0x00, 0x16, 0x1E);
            grayscale_6.set(0x00, 0x1D, 0x1D);
            grayscale_7.set(0x00, 0x23, 0x1F);
            grayscale_8.set(0x00, 0x2A, 0x1F);
            grayscale_9.set(0x00, 0x31, 0x1E);
            grayscale_10.set(0x00, 0x38, 0x1D);
            grayscale_11.set(0x00, 0x3E, 0x1F);
            grayscale_12.set(0x06, 0x3F, 0x1E);
            grayscale_13.set(0x0C, 0x3F, 0x1F);
            grayscale_14.set(0x13, 0x3F, 0x1E);
            grayscale_15.set(0x19, 0x3F, 0x1F);
            grayscale_16.set(0x1F, 0x3F, 0x1F);
        }

        // void get_grayscale_color(uint8_t color){
        //     lgfx::grayscale_t gs(color);
        //     gs.get();
        // }
    };
    extern resources resrc;
}