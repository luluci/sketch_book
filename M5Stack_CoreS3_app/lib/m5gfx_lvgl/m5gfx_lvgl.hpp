#ifndef __M5GFX_LVGL_H__
#define __M5GFX_LVGL_H__

#include <lvgl.h>
#include <M5Unified.h>
#include <M5GFX.h>

constexpr uint32_t LV_TICK_PERIOD_MS = 10;
constexpr size_t LCD_WIDTH = 320;
constexpr size_t LCD_HEIGHT = 240;

void m5gfx_lvgl_init(void);

#endif // __M5GFX_LVGL_H__
