#include "app_ble.hpp"

#include <lib_mini_appfx/event.hpp>
#include <tuple>

namespace app::pages
{
    app_ble::app_ble(id_type id_) : base_type::page(id_), panel(component_id::Panel)
    {
        // ポップアップ領域
        set_coord(0, 80, 540, 880);

        //
        add(panel);
        panel.set_coord(x, y, w, h);
    }
    bool app_ble::check_polling()
    {
        return false;
    }
    bool app_ble::on_touch_released(component_type *, int, int)
    {
        return false;
    }
}