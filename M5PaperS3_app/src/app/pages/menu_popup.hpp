#pragma once

#include <lib_mini_appfx/page.hpp>

#include "../components/popup.hpp"
#include "../page_id.hpp"
#include "../resources.hpp"

namespace app::pages
{

    enum class menu_popup_component_id
    {
        Popup
    };

    // ディスプレイ上部に配置するメニューバー
    class menu_popup : public lib_mini_appfx::page<page_id, menu_popup_component_id>
    {
        using id_type = page_id;
        using component_id = menu_popup_component_id;

    public:
        using base_type = lib_mini_appfx::page<page_id, component_id>;
        components::popup<component_id> popup;

        menu_popup(id_type id_);

        virtual bool check_polling() override;
        virtual bool on_click(component_type *) override;
    };
}
