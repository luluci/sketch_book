#include "app_power_manager.hpp"

#include <lib_mini_appfx/event.hpp>
#include <tuple>

namespace app::pages
{
    app_power_manager::app_power_manager(id_type id_) : base_type::page(id_), panel(component_id::Panel)
    {
        // ポップアップ領域
        set_coord(0, 80, 540, 880);

        //
        add(panel);
        panel.set_hit(false);
        panel.set_coord(0, 80, 540, 880);
    }
    bool app_power_manager::check_polling()
    {
        return false;
    }
    bool app_power_manager::on_touch_released(component_type *, int, int)
    {
        return false;
    }
}
