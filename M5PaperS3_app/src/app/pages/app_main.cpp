#include "app_main.hpp"

#include <lib_mini_appfx/event.hpp>
#include <tuple>

namespace app::pages
{
    app_main::app_main(id_type id_) : base_type::page(id_), panel(component_id::Panel)
    {
        // ポップアップ領域
        set_coord(0, 80, 540, 880);

        //
        add(panel);
        panel.set_hit(false);
        panel.set_coord(x, y, w, h);
    }
    bool app_main::check_polling()
    {
        return false;
    }
    bool app_main::on_touch_released(component_type *, int, int)
    {
        return false;
    }
}
