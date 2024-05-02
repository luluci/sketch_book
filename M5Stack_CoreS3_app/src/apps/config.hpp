#pragma once

#include "m5gfx_lvgl.hpp"

constexpr size_t SCR_HEADER_HEIGHT = 60;
constexpr size_t SCR_BODY_HEIGHT = LCD_HEIGHT - SCR_HEADER_HEIGHT;

constexpr size_t SCR_HEADER_Y = 0;
constexpr size_t SCR_BODY_Y = SCR_HEADER_Y + SCR_HEADER_HEIGHT;
