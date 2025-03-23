#pragma once

#include <lib_mini_appfx/page.hpp>

#include "../components/panel.hpp"
#include "../components/label.hpp"
#include "../page_id.hpp"
#include "../resources.hpp"

namespace app::pages
{

    enum class app_ble_component_id
    {
        Panel,
        AppCaption,
    };

    //
    class app_ble : public lib_mini_appfx::page<page_id, app_ble_component_id>
    {
        using id_type = page_id;
        using component_id = app_ble_component_id;

    public:
        using base_type = lib_mini_appfx::page<page_id, component_id>;
        components::panel<component_id> panel;
        components::label<component_id> app_caption;

        app_ble(id_type id_);

        virtual bool check_polling() override;
        virtual bool on_touch_pressed(component_type *, int, int) { return false; }
        virtual bool on_touch_dragging(component_type *, int, int, int, int) { return false; }
        virtual bool on_touch_released(component_type *, int, int) override;
    };
}
