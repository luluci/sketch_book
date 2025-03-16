#include "menu_popup.hpp"

#include <lib_mini_appfx/event.hpp>
#include <tuple>

namespace app::pages
{
    menu_popup::menu_popup(id_type id_) : base_type::page(id_), popup(component_id::Popup)
    {
        // ポップアップ領域
        set_coord(0, 80, 439, 699);

        //
        add(popup);
        popup.set_coord(0, 80, 439, 699);
    }
    bool menu_popup::check_polling()
    {
        return false;
    }
    bool menu_popup::on_click(component_type *)
    {
        return false;
    }
}