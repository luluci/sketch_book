#include "menu_header.hpp"

#include <lib_mini_appfx/event.hpp>
#include <tuple>

namespace app::pages
{
    menu_header::menu_header(id_type id_) : base_type::page(id_), menu_button(component_id::Menu), line(component_id::BorderLine)
    {
        // メニューバー領域
        set_coord(0, 0, 539, 79);

        // メニューボタン
        add(menu_button);
        menu_button.set_coord(5, 5, 100 - 10, 80 - 10);
        menu_button.set_text("MENU");
        menu_button.set_font(resrc.font_menu);

        // メニューバー区切り線
        add(line);
        line.set_hit(false);
        line.set_coord(0, 79 - 1, 539, 79);
    }
    bool menu_header::check_polling()
    {
        return false;
    }
    bool menu_header::on_click(component_type *component)
    {
        if (component->id == menu_button.id)
        {
            // ボタン押下
            // appとしてメニューを表示する
            req_popup(page_id::MenuPopup, 0);
            return true;
        }

        return false;
    }
}