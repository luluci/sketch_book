#include "app_ble.hpp"

#include <lib_mini_appfx/event.hpp>
#include <tuple>

namespace app::pages
{
    app_ble::app_ble(id_type id_) : base_type::page(id_), panel(component_id::Panel), app_caption(component_id::AppCaption)
    {
        // ポップアップ領域
        set_coord(0, 80, 540, 880);

        //
        add(panel);
        panel.set_hit(false);
        panel.set_coord(x, y, w, h);

        // label
        add(app_caption);
        app_caption.set_hit(false);
        app_caption.set_underline(true);
        app_caption.set_bkcolor(resrc.grayscale_14);
        app_caption.set_round(3);
        app_caption.set_coord(x + 5, y + 5, 200, 50);
        app_caption.set_text("Bluetooth/BLE App");
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
