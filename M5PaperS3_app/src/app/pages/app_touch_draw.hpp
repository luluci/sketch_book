#pragma once

#include <lib_mini_appfx/page.hpp>

#include "../components/draw_canvas.hpp"
#include "../components/button.hpp"
#include "../page_id.hpp"
#include "../resources.hpp"

namespace app::pages
{

    enum class app_touch_draw_component_id
    {
        Canvas,
        ClearButton,
    };

    //
    class app_touch_draw : public lib_mini_appfx::page<page_id, app_touch_draw_component_id>
    {
        using id_type = page_id;
        using component_id = app_touch_draw_component_id;

    public:
        using base_type = lib_mini_appfx::page<page_id, component_id>;
        components::draw_canvas<component_id> canvas;
        components::button<component_id> clear_button;

        static constexpr int32_t app_h = 880;
        static constexpr int32_t canvas_h = 820;
        static constexpr int32_t button_h = app_h - canvas_h;

        app_touch_draw(id_type id_);

        virtual bool check_polling() override;
        virtual bool on_touch_released(component_type *, int, int) override;

        // 表示app変更通知
        virtual void on_change_app(id_type new_app) override
        {
            // nothing
        }
    };
}
