#pragma once
// clang-format off
#include <M5Unified.h>
// clang-format on

#include <lib_mini_appfx.hpp>
#include <esp_timer.h>

#include "page_id.hpp"
#include "pages/menu_header.hpp"
#include "pages/menu_popup.hpp"
#include "pages/app_main.hpp"
#include "pages/app_power_manager.hpp"
#include "pages/app_test_epdiy.hpp"
#include "pages/app_touch_draw.hpp"
#include "pages/app_ble.hpp"

namespace app
{

    class app : public lib_mini_appfx::app<page_id>
    {
        // app周期処理間隔(ms)
        uint32_t cycle_time_ms_ = 100;

        // canvas使う？
        // 5Canvas canvas;

        // menu
        pages::menu_header menu_header;
        pages::menu_popup menu_popup;
        // app
        pages::app_main app_main;
        pages::app_power_manager app_power_manager;
        pages::app_test_epdiy app_test_epdiy;
        pages::app_touch_draw app_touch_draw;
        pages::app_ble app_ble;

    public:
        using base_type = lib_mini_appfx::app<page_id>;
        // using base_type::app;

        app();

        void init();

        uint32_t operator()();

    private:
        bool check_event();
        bool check_event_touch();

        virtual void begin_render() override;
        virtual void end_render() override;
        // 表示app変更通知
        virtual void on_change_app(id_type new_app) override;
    };
}

extern app::app m5app;
