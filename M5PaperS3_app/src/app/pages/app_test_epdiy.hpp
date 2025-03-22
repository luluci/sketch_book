#pragma once

#include <lib_mini_appfx/page.hpp>

#include "../components/test.hpp"
#include "../page_id.hpp"
#include "../resources.hpp"

namespace app::pages
{

    enum class app_test_epdiy_component_id
    {
        Panel
    };

    //
    class app_test_epdiy : public lib_mini_appfx::page<page_id, app_test_epdiy_component_id>
    {
        using id_type = page_id;
        using component_id = app_test_epdiy_component_id;

    public:
        using base_type = lib_mini_appfx::page<page_id, component_id>;
        components::panel_test<component_id> panel;

        app_test_epdiy(id_type id_);

        virtual bool check_polling() override;
        virtual bool on_click(component_type *) override;

        // 表示app変更通知
        virtual void on_change_app(id_type new_app) override
        {
            // nothing
        }
    };
}
