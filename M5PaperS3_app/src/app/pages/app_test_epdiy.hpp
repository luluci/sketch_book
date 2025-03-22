#pragma once

#include <lib_mini_appfx/page.hpp>

#include "../components/test.hpp"
#include "../components/panel.hpp"
#include "../components/button.hpp"
#include "../page_id.hpp"
#include "../resources.hpp"

namespace app::pages
{

    enum class app_test_epdiy_component_id
    {
        TestDispPanel,
        NavigatorPanel,
        PrevButton,
        NextButton,
        PageText,
    };

    //
    class app_test_epdiy : public lib_mini_appfx::page<page_id, app_test_epdiy_component_id>
    {
        using id_type = page_id;
        using component_id = app_test_epdiy_component_id;

    public:
        using base_type = lib_mini_appfx::page<page_id, component_id>;
        components::panel_test<component_id> test_disp_panel;
        components::panel<component_id> navi_panel;
        components::button<component_id> prev_button;
        components::button<component_id> next_button;

        app_test_epdiy(id_type id_);

        virtual bool check_polling() override;
        virtual bool on_touch_pressed(component_type *, int, int) { return false; }
        virtual bool on_touch_dragging(component_type *, int, int, int, int) { return false; }
        virtual bool on_touch_released(component_type *, int, int) override;
    };
}
