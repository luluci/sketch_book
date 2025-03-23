#pragma once

#include <lib_mini_appfx/page.hpp>

#include "../components/panel.hpp"
#include "../components/button.hpp"
#include "../components/label.hpp"
#include "../page_id.hpp"
#include "../resources.hpp"

namespace app::pages
{

    enum class app_ble_component_id
    {
        Panel,
        AppCaption,
        BLEServerStart,
        BLEServerStop,
        BLEConnectStatus,
    };

    //
    class app_ble : public lib_mini_appfx::page<page_id, app_ble_component_id>
    {
        using id_type = page_id;
        using component_id = app_ble_component_id;

        enum class ble_status
        {
            Disconnect, // 初期状態
            Connecting, // 接続中
            Connected,  // 接続状態
        };
        ble_status status;

        static constexpr char const *const ble_status_label[] =
            {
                "Disconnect",
                "Connecting",
                "Connected",
            };

    public:
        using base_type = lib_mini_appfx::page<page_id, component_id>;
        using label_t = components::label<component_id>;
        components::panel<component_id> panel;
        label_t app_caption;
        components::button<component_id> ble_server_start;
        components::button<component_id> ble_server_stop;
        label_t ble_connect_status;

        app_ble(id_type id_);

        virtual bool check_polling() override;
        virtual bool on_touch_pressed(component_type *, int, int) { return false; }
        virtual bool on_touch_dragging(component_type *, int, int, int, int) { return false; }
        virtual bool on_touch_released(component_type *, int, int) override;

        // BLE処理
        void on_connect_button();
        void check_event_ble_server();
        void check_event_ble_data_trans();
    };
}
