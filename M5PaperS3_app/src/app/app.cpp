#include "app.hpp"

app::app m5app;

namespace app
{
    app::app() : menu_header(page_id::MenuHeader),
                 menu_popup(page_id::MenuPopup),
                 app_main(page_id::AppMain),
                 app::base_type(menu_header, menu_popup, app_main)
    {
        m5app.init(page_id::MenuHeader, page_id::AppMain);
    }

    size_t app::operator()()
    {
        // 開始時刻取得
        size_t time_ms = (esp_timer_get_time() / 1000ULL);
        M5.update();

        //
        if (check_event())
        {
            // イベント処理済み
        }
        else
        {
            // イベント無し周期処理
            base_type::polling();
        }

        // 処理時間を差し引いた時間をwaitして周期時間を可能な範囲で守る
        size_t proced_time_ms = (esp_timer_get_time() / 1000ULL);
        return (cycle_time_ms_ - (proced_time_ms - time_ms));
    }

    bool app::check_event()
    {
        // タッチ操作チェック
        if (check_event_touch())
        {
            return true;
        }
        return false;
    }

    bool app::check_event_touch()
    {
        m5::Touch_Class::touch_detail_t const &touch = M5.Touch.getDetail();
        // if (touch.isFlicking())
        // {
        // }
        // if (touch.wasFlicked())
        // {
        // }
        // if (touch.wasPressed())
        // {
        // }
        if (touch.wasReleased())
        {
            check_click(touch.x, touch.y);
            return true;
        }

        return false;
    }
}