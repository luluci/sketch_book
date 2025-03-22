#include "menu_popup.hpp"

#include <lib_mini_appfx/event.hpp>
#include <tuple>

namespace app::pages
{
    menu_popup::menu_popup(id_type id_)
        : base_type::page(id_),
          popup(component_id::Popup),
          menu_power(component_id::MenuPower),
          menu_test_epdiy(component_id::MenuTestEPDiy)
    {
        // ポップアップ領域
        set_coord(0, 80, 440, 700);

        // メニュー領域
        add(popup);
        popup.set_coord(x, y, w, h);
        popup.set_hit(false);
        // メニューアイテム
        int menu_count = 0;
        //
        add(menu_power);
        menu_power.set_coord(x + padding, y + padding + (menu_item_h * menu_count), w - (padding * 2), menu_item_h);
        menu_power.set_text("Power Manager");
        menu_power.set_font(resrc.font_menu);
        menu_count++;
        //
        add(menu_test_epdiy);
        menu_test_epdiy.set_coord(x + padding, y + padding + (menu_item_h * menu_count), w - (padding * 2), menu_item_h);
        menu_test_epdiy.set_text("test EPDiy");
        menu_test_epdiy.set_font(resrc.font_menu);
        menu_count++;
    }
    bool menu_popup::check_polling()
    {
        return false;
    }
    bool menu_popup::on_click(component_type *component)
    {
        if (component->id == menu_power.id)
        {
        }
        if (component->id == menu_test_epdiy.id)
        {
            // appとしてを表示する
            req_close_popup(0);
            req_change(page_id::AppTestEPDiy, 0);
            return true;
        }

        return false;
    }
}