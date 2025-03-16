#pragma once

#include <lib_mini_appfx/page.hpp>

#include "../components/button_label.hpp"
#include "../components/line.hpp"
#include "../page_id.hpp"
#include "../resources.hpp"

namespace app::pages
{

    enum class menu_header_component_id
    {
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

        components::button_label<component_id> menu_button;
        components::line<component_id> line;

        menu_header(id_type id_);

        virtual bool check_polling() override;
        virtual bool on_click(component_type *) override;
    };
}
