#pragma once

#include <lib_mini_appfx/page.hpp>

#include "../components/app.hpp"
#include "../page_id.hpp"
#include "../resources.hpp"

namespace app::pages
{

    enum class app_main_component_id
    {
        App
    };

    // ディスプレイ上部に配置するメニューバー
    class app_main : public lib_mini_appfx::page<page_id, app_main_component_id>
    {
        using id_type = page_id;
        using component_id = app_main_component_id;

    public:
        using base_type = lib_mini_appfx::page<page_id, component_id>;
        components::app<component_id> app;

        app_main(id_type id_);

        virtual bool check_polling() override;
        virtual bool on_click(component_type *) override;
    };
}
