#pragma once

#include <lib_mini_appfx/page.hpp>

#include "../components/panel.hpp"
#include "../components/popup.hpp"
#include "../components/menu_item.hpp"
#include "../page_id.hpp"
#include "../resources.hpp"

namespace app::pages
{

    enum class menu_popup_component_id
    {
        Popup,
        MenuPower,
        MenuSettings,
        MenuBLE,
        MenuTestEPDiy,
        MenuDrawCanvas,
    };

    //
    class menu_popup : public lib_mini_appfx::page<page_id, menu_popup_component_id>
    {
        using id_type = page_id;
        using component_id = menu_popup_component_id;

    public:
        using base_type = lib_mini_appfx::page<page_id, component_id>;
        // components::panel<component_id> panel;
        components::popup<component_id> popup;
        components::menu_item<component_id> menu_power;
        components::menu_item<component_id> menu_settings;
        components::menu_item<component_id> menu_ble;
        components::menu_item<component_id> menu_test_epdiy;
        components::menu_item<component_id> menu_draw_canvas;

        using base_type::h;
        using base_type::w;
        using base_type::x;
        using base_type::x2;
        using base_type::y;
        using base_type::y2;
        static constexpr int32_t padding = components::popup<component_id>::padding + 2;
        static constexpr int32_t menu_item_h = 80;

        menu_popup(id_type id_);

        virtual bool check_polling() override;
        virtual bool on_touch_pressed(component_type *, int, int) { return false; }
        virtual bool on_touch_dragging(component_type *, int, int, int, int) { return false; }
        virtual bool on_touch_released(component_type *, int, int) override;
    };
}
