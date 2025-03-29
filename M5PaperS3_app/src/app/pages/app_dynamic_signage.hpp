#pragma once

#include <lib_mini_appfx/page.hpp>

#include "../components/panel.hpp"
#include "../components/label.hpp"
#include "../page_id.hpp"
#include "../resources.hpp"

namespace app::pages
{

    enum class app_dynamic_signage_component_id
    {
        Panel,
        LabelDest,   // 行き先表示
        LabelDepart, // 部署名表示
        LabelName,   // 名前表示
    };

    //
    class app_dynamic_signage : public lib_mini_appfx::page<page_id, app_dynamic_signage_component_id>
    {
        using id_type = page_id;
        using component_id = app_dynamic_signage_component_id;

        uint8_t rotate_org_;

    public:
        using base_type = lib_mini_appfx::page<page_id, component_id>;
        components::panel<component_id> panel;
        components::label<component_id> label_dest;   // 行き先表示
        components::label<component_id> label_depart; // 部署名表示
        components::label<component_id> label_name;   // 名前表示

        app_dynamic_signage(id_type id_);

        virtual void begin_render(uint32_t data) override;
        virtual void end_render(uint32_t data) override;

        virtual bool check_polling() override;
        virtual bool on_touch_pressed(component_type *, int, int) { return false; }
        virtual bool on_touch_dragging(component_type *, int, int, int, int) { return false; }
        virtual bool on_touch_released(component_type *, int, int) override;
    };
}
