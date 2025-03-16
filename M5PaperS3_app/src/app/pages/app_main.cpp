#include "app_main.hpp"

#include <lib_mini_appfx/event.hpp>
#include <tuple>

namespace app::pages
{
    app_main::app_main(id_type id_) : base_type::page(id_), app(component_id::App)
    {
        // ポップアップ領域
        set_coord(0, 80, 539, 960);

        //
        add(app);
        app.set_coord(0, 80, 539, 960);
    }
    bool app_main::check_polling()
    {
        return false;
    }
    bool app_main::on_click(component_type *)
    {
        return false;
    }
}