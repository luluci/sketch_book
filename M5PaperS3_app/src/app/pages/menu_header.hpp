#pragma once

#include <lib_mini_appfx/page.hpp>

#include "../components/panel.hpp"
#include "../components/button_label.hpp"
#include "../components/line.hpp"
#include "../page_id.hpp"
#include "../resources.hpp"

namespace app::pages
{

    enum class menu_header_component_id
    {
        Base,
        Menu,
        BorderLine
    };

    // ディスプレイ上部に配置するメニューバー
    class menu_header : public lib_mini_appfx::page<page_id, menu_header_component_id>
    {
        using id_type = page_id;
        using component_id = menu_header_component_id;

    public:
        using base_type = lib_mini_appfx::page<page_id, component_id>;

        components::panel<component_id> panel;
        components::button_label<component_id> menu_button;
        components::line<component_id> line;
        bool is_show;

        menu_header(id_type id_);

        virtual bool check_polling() override;
        virtual bool on_click(component_type *) override;

        // 表示app変更通知
        virtual void on_change_app(id_type new_app) override;
    };
}
