#include "app_ble.hpp"

#include <lib_mini_appfx/event.hpp>
#include <tuple>

#include "ble_paper_s3.hpp"

namespace app::pages
{
    app_ble::app_ble(id_type id_)
        : base_type::page(id_),
          status(ble_status::Disconnect),
          panel(component_id::Panel),
          app_caption(component_id::AppCaption),
          ble_server_start(component_id::BLEServerStart),
          ble_server_stop(component_id::BLEServerStop),
          ble_connect_status(component_id::BLEConnectStatus)
    {
        // ポップアップ領域
        set_coord(0, 80, 540, 880);

        //
        add(panel);
        panel.set_hit(false);
        panel.set_coord(x, y, w, h);

        // label
        add(app_caption);
        app_caption.set_hit(false);
        app_caption.set_underline(true);
        app_caption.set_coord(x, y, 260, 50);
        app_caption.set_text("Bluetooth/BLE App");
        app_caption.set_font(resrc.font_menu);

        // BLE Server動作開始 ボタン
        add(ble_server_start);
        ble_server_start.set_hit(true);
        ble_server_start.set_underline(true);
        ble_server_start.set_bkcolor(resrc.grayscale_16);
        ble_server_start.set_border_color(resrc.grayscale_0);
        ble_server_start.set_round(10);
        ble_server_start.set_coord(x + 20, y + 60, 240, 70);
        ble_server_start.set_text("BLE Start");
        ble_server_start.set_font(resrc.font_menu);
        // BLE Server動作停止 ボタン
        add(ble_server_stop);
        ble_server_stop.set_hit(true);
        ble_server_stop.set_underline(false);
        ble_server_stop.set_bkcolor(resrc.grayscale_10);
        ble_server_stop.set_border_color(resrc.grayscale_0);
        ble_server_stop.set_round(10);
        ble_server_stop.set_coord(x + 280, y + 60, 240, 70);
        ble_server_stop.set_text("BLE Stop");
        ble_server_stop.set_font(resrc.font_menu);

        // Advertising状態
        // 接続状態
        add(ble_connect_status);
        ble_connect_status.set_hit(true);
        ble_connect_status.set_coord(x + 20, y + 140, 320, 70);
        ble_connect_status.set_align(label_t::text_alignment::Left);
        ble_connect_status.set_text(ble_status_label[(size_t)status]);
        ble_connect_status.set_font(resrc.font_menu);

        // data_trans状態
    }
    bool app_ble::check_polling()
    {
        // 常時処理
        check_event_ble_server();

        // アクティブのときだけ表示更新
        if (is_active)
        {
        }

        return false;
    }
    bool app_ble::on_touch_released(component_type *component, int, int)
    {
        if (component->id == ble_server_start.id)
        {
            on_connect_button();
        }
        return false;
    }

    void app_ble::on_connect_button()
    {
        switch (status)
        {
        case ble_status::Disconnect:
            // 未接続: 接続手続き開始
            // BLEサーバーを起動
            BLE.start();
            status = ble_status::Connecting;
            break;

        case ble_status::Connecting:
        case ble_status::Connected:
        default:
            // その他
            // BLEサーバーを停止
            BLE.stop();
            status = ble_status::Disconnect;
            break;
        }
    }
    void app_ble::check_event_ble_server()
    {
        // BLE serverのイベント処理
        auto event = BLE.get_event();

        if (event != 0)
        {
            if ((event & ble::server_event::onConnect) != 0)
            {
                // M5.Lcd.println("onConnect");
            }
            if ((event & ble::server_event::onDisconnect) != 0)
            {
                // M5.Lcd.println("onDisconnect");
            }
            if ((event & ble::server_event::onPassKeyRequest) != 0)
            {
                // M5.Lcd.printf("onPassKeyRequest : %d\n", BLE.get_pass_key());
            }
            if ((event & ble::server_event::onPassKeyNotify) != 0)
            {
                // M5.Lcd.printf("onPassKeyNotify : %d\n", BLE.get_pass_key());
            }
            if ((event & ble::server_event::onSecurityRequest) != 0)
            {
                // M5.Lcd.println("onSecurityRequest");
            }
            if ((event & ble::server_event::onAuthenticationComplete) != 0)
            {
                // M5.Lcd.println("onAuthenticationComplete");
            }
            if ((event & ble::server_event::onConfirmPIN) != 0)
            {
                // M5.Lcd.println("onConfirmPIN");
            }
        }
    }
    void app_ble::check_event_ble_data_trans()
    {
        // BLE service: data_transのイベント処理
        auto event = ble_data_trans.get_event();
    }
}
