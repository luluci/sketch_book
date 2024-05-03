#pragma once

#include <lvgl.h>
#include "m5gfx_lvgl.hpp"

namespace app
{
	constexpr size_t SCR_WIDTH = LCD_WIDTH;
	constexpr size_t SCR_HEIGHT = LCD_HEIGHT;

	constexpr size_t SCR_HEADER_HEIGHT = 60;
	constexpr size_t SCR_BODY_HEIGHT = LCD_HEIGHT - SCR_HEADER_HEIGHT;

	constexpr size_t SCR_HEADER_Y = 0;
	constexpr size_t SCR_BODY_Y = SCR_HEADER_Y + SCR_HEADER_HEIGHT;

	enum class item : size_t
	{
		timer,

		MAX
	};

	extern lv_style_t style_base_transp;
	extern lv_style_t style_base_black;

	// 独自イベント
	extern uint32_t APP_EVENT_MENU_SELECTED;
}