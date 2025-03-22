#include "app_test_epdiy.hpp"

#include <lib_mini_appfx/event.hpp>
#include <tuple>

namespace app::pages
{
    app_test_epdiy::app_test_epdiy(id_type id_) : base_type::page(id_), panel(component_id::Panel)
    {
        // ポップアップ領域
        set_coord(0, 80, 540, 880);

        //
        add(panel);
        panel.set_hit(false);
        panel.set_coord(x, y, w, h);
    }
    bool app_test_epdiy::check_polling()
    {
        return false;
    }
    bool app_test_epdiy::on_click(component_type *)
    {
        return false;
    }
}