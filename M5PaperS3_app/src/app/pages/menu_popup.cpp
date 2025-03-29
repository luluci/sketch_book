#include "menu_popup.hpp"

#include <lib_mini_appfx/event.hpp>
#include <tuple>

namespace app::pages
{
    menu_popup::menu_popup(id_type id_)
        : base_type::page(id_),
          popup(component_id::Popup),
          menu_power(component_id::MenuPower),
          menu_settings(component_id::MenuSettings),
          menu_ble(component_id::MenuBLE),
          menu_test_epdiy(component_id::MenuTestEPDiy),
          menu_draw_canvas(component_id::MenuDrawCanvas),
          menu_dynamic_signage(component_id::MenuDynamicSignage)
    {
        // ポップアップ領域
        set_coord(0, 80, 440, 700);

        // メニュー領域
        add(popup);
        popup.set_hit(false);
        popup.set_coord(x, y, w, h);
        // メニューアイテム
        int menu_count = 0;
        // 電源app表示
        add(menu_power);
        menu_power.set_coord(x + padding, y + padding + (menu_item_h * menu_count), w - (padding * 2), menu_item_h);
        menu_power.set_text("Power Manager");
        menu_power.set_font(resrc.font_menu);
        menu_count++;
        // Settings app表示
        add(menu_settings);
        menu_settings.set_coord(x + padding, y + padding + (menu_item_h * menu_count), w - (padding * 2), menu_item_h);
        menu_settings.set_text("Settings");
        menu_settings.set_font(resrc.font_menu);
        menu_count++;
        // Settings app表示
        add(menu_ble);
        menu_ble.set_coord(x + padding, y + padding + (menu_item_h * menu_count), w - (padding * 2), menu_item_h);
        menu_ble.set_text("BLE");
        menu_ble.set_font(resrc.font_menu);
        menu_count++;
        // EPDiy テストapp
        add(menu_test_epdiy);
        menu_test_epdiy.set_coord(x + padding, y + padding + (menu_item_h * menu_count), w - (padding * 2), menu_item_h);
        menu_test_epdiy.set_text("test EPDiy");
        menu_test_epdiy.set_font(resrc.font_menu);
        menu_count++;
        // お絵描きapp
        add(menu_draw_canvas);
        menu_draw_canvas.set_coord(x + padding, y + padding + (menu_item_h * menu_count), w - (padding * 2), menu_item_h);
        menu_draw_canvas.set_text("Draw Canvas");
        menu_draw_canvas.set_font(resrc.font_menu);
        menu_count++;
        // 動的サイネージapp
        add(menu_dynamic_signage);
        menu_dynamic_signage.set_coord(x + padding, y + padding + (menu_item_h * menu_count), w - (padding * 2), menu_item_h);
        menu_dynamic_signage.set_text("Signage");
        menu_dynamic_signage.set_font(resrc.font_menu);
        menu_count++;
    }
    bool menu_popup::check_polling()
    {
        return false;
    }
    bool menu_popup::on_touch_released(component_type *component, int, int)
    {
        if (component->id == menu_power.id)
        {
        }
        if (component->id == menu_ble.id)
        {
            // appとしてを表示する
            req_close_popup(0);
            req_change(page_id::AppBLE, 0);
            return true;
        }
        if (component->id == menu_test_epdiy.id)
        {
            // appとしてを表示する
            req_close_popup(0);
            req_change(page_id::AppTestEPDiy, 0);
            return true;
        }
        if (component->id == menu_draw_canvas.id)
        {
            // appとしてを表示する
            req_close_popup(0);
            req_change(page_id::AppDrawCanvas, 0);
            return true;
        }
        if (component->id == menu_dynamic_signage.id)
        {
            // appとしてを表示する
            req_close_popup(0);
            req_change(page_id::AppSignage, 0);
            return true;
        }

        return false;
    }
}
