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

namespace app
{

    class app : public lib_mini_appfx::app<page_id>
    {
        size_t cycle_time_ms_ = 100;

        // canvas使う？
        // 5Canvas canvas;

        // menu
        pages::menu_header menu_header;
        pages::menu_popup menu_popup;
        // app
        pages::app_main app_main;

    public:
        using base_type = lib_mini_appfx::app<page_id>;
        // using base_type::app;

        app();

        void init();

        size_t operator()();

    private:
        bool check_event();
        bool check_event_touch();

        virtual void render_begin() override;
        virtual void render_end() override;
    };
}

extern app::app m5app;
