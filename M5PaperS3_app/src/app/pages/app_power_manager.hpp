#pragma once

#include <lib_mini_appfx/page.hpp>

#include "../components/panel.hpp"
#include "../page_id.hpp"
#include "../resources.hpp"

namespace app::pages
{

    enum class app_power_manager_component_id
    {
        Panel
    };

    //
    class app_power_manager : public lib_mini_appfx::page<page_id, app_power_manager_component_id>
    {
        using id_type = page_id;
        using component_id = app_power_manager_component_id;

    public:
        using base_type = lib_mini_appfx::page<page_id, component_id>;
        components::panel<component_id> panel;

        app_power_manager(id_type id_);

        virtual bool check_polling() override;
        virtual bool on_click(component_type *) override;

        // 表示app変更通知
        virtual void on_change_app(id_type new_app) override
        {
            // nothing
        }
    };
}
