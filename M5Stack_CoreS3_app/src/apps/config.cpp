#include "config.hpp"

namespace app
{
	lv_style_t style_base_transp;
	lv_style_t style_base_black;

	// 独自イベント
	uint32_t APP_EVENT_MENU_SELECTED = lv_event_register_id();
}
