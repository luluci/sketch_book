#include "menu_header.hpp"

#include <lib_mini_appfx/event.hpp>
#include <tuple>

namespace app::pages
{
    menu_header::menu_header(id_type id_)
        : base_type::page(id_),
          panel(component_id::Base),
          menu_button(component_id::Menu),
          line(component_id::BorderLine), is_show(false)
    {
        // メニューバー領域
        set_coord(0, 0, 540, 80);

        // メニューベース
        add(panel);
        panel.set_hit(false);
        panel.set_coord(0, 0, 540, 80);

        // メニューボタン
        add(menu_button);
        menu_button.set_coord(5, 5, 120 - 10, 80 - 10);
        menu_button.set_text("MENU");
        menu_button.set_font(resrc.font_menu);

        // メニューバー区切り線
        add(line);
        line.set_hit(false);
        line.set_coord(0, 80 - 5 - 1, 540, 5);
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
            if (is_show)
            {
                // メニュー表示中
                req_close_popup(0);
            }
            else
            {
                // メニュー非表示
                // appとしてメニューを表示する
                req_popup(page_id::MenuPopup, 0);
            }
            is_show = !is_show;
            return true;
        }

        return false;
    }
    void menu_header::on_change_app(id_type new_app)
    {
        // 表示appが変更された
        // メニュー表示中
        if (is_show)
        {
            // メニューが閉じられた
            if (new_app != page_id::MenuPopup)
            {
                //
            }
        }

        // base_type::render(true, 0);
    }
}