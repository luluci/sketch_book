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

        resources() : font_default(&fonts::lgfxJapanGothic_16), font_menu(&fonts::DejaVu18)
        {
        }
    };
    extern resources resrc;
}