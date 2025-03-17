// clang-format off
#include <epdiy.h>
#include <M5Unified.h>
// clang-format on

#include "ble_paper_s3.hpp"

#include "app/app.hpp"

void app__();
void check_ble_pairing();
void check_event();
void check_event_touch();
void check_event_ble_server();
void check_event_ble_serial();
void check_event_ble_data_trans();
void check_psram();

// app周期
constexpr uint32_t app_cycle = 100;

size_t lcd_line_count = 0;

void setup()
{
    try
    {
        // m5::M5Unified::config_t cfg
        M5.begin();

        //
        M5.Speaker.begin();
        M5.Speaker.stop();

        // LCD
        M5.Lcd.setRotation(2);

        M5.Lcd.setTextSize(3);
        M5.Lcd.setCursor(5, 5);
        M5.Lcd.println("Init Start.");

        M5.Lcd.println("Init BLE.");
        BLE.add_service(&ble_serial);
        BLE.add_service(&ble_data_trans);
        BLE.setup("M5PaperS3");

        M5.Lcd.println("Init Fin.");
        check_psram();

        // app初期化
        m5app.init();
        // app構築に失敗したら
        if (!m5app.is_build_ok)
        {
            M5.Lcd.printf("GUI page size: %d\n", m5app.pages.size());
            for (int i = 0; i < m5app.pages.size(); i++)
            {
                auto &page = m5app.pages[i];
                M5.Lcd.printf(" page[%d]: id=%d\n", i, page->id);
            }
        }
    }
    catch (const std::runtime_error &e)
    {
        M5.Lcd.printf("setup() exception occur!\n");
        M5.Lcd.printf("msg: %s\n", e.what());
        while (true)
        {
            delay(10000);
        }
    }
}

void loop()
{
    try
    {

        // M5.update();

        // app__();

        // delay(app_cycle);

        // M5.update();
        delay(m5app());
    }
    catch (const std::runtime_error &e)
    {
        M5.Lcd.printf("loop() exception occur!\n");
        M5.Lcd.printf("msg: %s\n", e.what());
        while (true)
        {
            delay(10000);
        }
    }
}

void start_disp()
{
    M5.Lcd.startWrite();
    if (lcd_line_count > 30)
    {
        lcd_line_count = 0;
        M5.Lcd.clearDisplay();
        M5.Lcd.setCursor(5, 5);
    }
}
void end_disp()
{
    M5.Lcd.endWrite();
}

enum class app_state
{
    InitWait,
    WaitBlePairing,
    Running,
};
app_state state = app_state::InitWait;
size_t timer = 0;
size_t ble_data_trans_timer = 0;

void app__()
{
    switch (state)
    {
    case app_state::InitWait:
        timer++;
        if (timer > 10)
        {
            timer = 0;
            state = app_state::WaitBlePairing;
            // BLEサーバーを起動
            BLE.start();
            //
            M5.Lcd.startWrite();
            M5.Lcd.clearDisplay();
            M5.Lcd.setCursor(5, 5);
            M5.Lcd.println("BLE Start. Wait Pairing.");
            M5.Lcd.print("PIN: ");
            M5.Lcd.println(BLE.get_device_addr_str());
            //
            M5.Lcd.endWrite();
        }
        break;

    case app_state::WaitBlePairing:
        check_ble_pairing();
        break;

    case app_state::Running:
        check_event();
        break;
    }

    // touch
    check_event_touch();
}

void check_ble_pairing()
{
    if (BLE.is_connected())
    {
        //
        M5.Lcd.startWrite();
        M5.Lcd.println("BLE Pairing Fin.");
        M5.Lcd.endWrite();
        state = app_state::Running;
    }
    else
    {
    }
}

void check_event()
{
    // service event
    check_event_ble_server();
    // serial event
    check_event_ble_server();
    // data_trans event
    ble_data_trans.polling(app_cycle);
    check_event_ble_data_trans();
}

bool is_flicking = false;
void check_event_touch()
{
    m5::Touch_Class::touch_detail_t const &touch = M5.Touch.getDetail();
    if (touch.isFlicking())
    {
        if (!is_flicking)
        {
            is_flicking = true;
            M5.Lcd.println("isFlicking");
            lcd_line_count++;
        }
    }
    if (touch.wasFlicked())
    {
        is_flicking = false;
        M5.Lcd.println("wasFlicked");
        lcd_line_count++;
    }
    if (touch.wasPressed())
    {
        M5.Lcd.println("wasPressed");
        lcd_line_count++;
    }
    if (touch.wasReleased())
    {
        M5.Lcd.println("wasReleased");
        lcd_line_count++;
    }
}

void check_event_ble_server()
{
    auto event = BLE.get_event();
    if (event != 0)
    {
        start_disp();

        if ((event & ble::server_event::onConnect) != 0)
        {
            M5.Lcd.println("onConnect");
            lcd_line_count++;
        }
        if ((event & ble::server_event::onDisconnect) != 0)
        {
            M5.Lcd.println("onDisconnect");
            lcd_line_count++;
        }
        if ((event & ble::server_event::onPassKeyRequest) != 0)
        {
            M5.Lcd.printf("onPassKeyRequest : %d\n", BLE.get_pass_key());
            lcd_line_count++;
        }
        if ((event & ble::server_event::onPassKeyNotify) != 0)
        {
            M5.Lcd.printf("onPassKeyNotify : %d\n", BLE.get_pass_key());
            lcd_line_count++;
        }
        if ((event & ble::server_event::onSecurityRequest) != 0)
        {
            M5.Lcd.println("onSecurityRequest");
            lcd_line_count++;
        }
        if ((event & ble::server_event::onAuthenticationComplete) != 0)
        {
            M5.Lcd.println("onAuthenticationComplete");
            lcd_line_count++;
        }
        if ((event & ble::server_event::onConfirmPIN) != 0)
        {
            M5.Lcd.println("onConfirmPIN");
            lcd_line_count++;
        }

        end_disp();
    }
}

void check_event_ble_serial()
{
    auto event = ble_serial.get_event();
    if (event != 0)
    {
        start_disp();

        if ((event & (uint16_t)ble::service::serial_event::HasRecieve) != 0)
        {
            // 受信処理
            for (size_t i = 0; i < ble_serial.rx_buffer_index; i++)
            {
                M5.Lcd.println(ble_serial.rx_buffer[i].c_str());
                lcd_line_count++;
            }
            ble_serial.rx_buffer_index = 0;
            check_psram();
        }
        if ((event & (uint16_t)ble::service::serial_event::HasRead) != 0)
        {
            M5.Lcd.println("BLE Serial any read occur.");
            lcd_line_count++;
        }
        if ((event & (uint16_t)ble::service::serial_event::HasNotify) != 0)
        {
            M5.Lcd.println("BLE Serial Notify occur.");
            lcd_line_count++;
        }

        end_disp();
    }
}

void check_event_ble_data_trans()
{
    auto event = ble_data_trans.get_event();
    if (event != 0)
    {
        start_disp();

        //
        if ((event & (uint16_t)ble::service::data_trans_event::DataRecvFailed) != 0)
        {
            event = 0;
            M5.Lcd.println("DataRecvFailed");
            lcd_line_count++;
            M5.Lcd.println(ble_data_trans.err_msg);
            lcd_line_count++;
        }
        if ((event & (uint16_t)ble::service::data_trans_event::DataRecvTimeout) != 0)
        {
            event = 0;
            M5.Lcd.println("DataRecvTimeout");
            lcd_line_count++;
            M5.Lcd.println(ble_data_trans.err_msg);
            lcd_line_count++;
        }
        if ((event & (uint16_t)ble::service::data_trans_event::PSRAMAllocFailed) != 0)
        {
            event = 0;
            M5.Lcd.println("PSRAMAllocFailed");
            lcd_line_count++;
            M5.Lcd.println(ble_data_trans.err_msg);
            lcd_line_count++;
        }

        // データ転送開始
        if ((event & (uint16_t)ble::service::data_trans_event::DataRecvStart) != 0)
        {
            M5.Lcd.println("DataRecvStart");
            lcd_line_count++;
        }
        // データ受信中
        if ((event & (uint16_t)ble::service::data_trans_event::DataRecving) != 0)
        {
            ble_data_trans_timer++;
            if (ble_data_trans_timer > 10)
            {
                ble_data_trans_timer = 0;
                M5.Lcd.printf("DataRecing: %d / %d\n", ble_data_trans.recv_page, ble_data_trans.declare_buff.data.body.page_max);
                lcd_line_count++;
            }
        }
        // データ受信完了
        if ((event & (uint16_t)ble::service::data_trans_event::DataRecvSuccess) != 0)
        {
            if (ble_data_trans.declare_buff.data.header.data_type == ble::service::data_trans_data_type::file_PNG)
            {
                // BLEで取得したデータをunique_ptrのmoveで取得する
                // 代わりにすでに取得済みunique_ptrがあるなら再利用するように返す
                // ble_data_png_size = ble_data_trans.get_buff_size();
                // ble_data_png = ble_data_trans.get_buff();
                ble_data_trans.get_buff(ble_data_png, ble_data_png_size);
                if (ble_data_png)
                {
                    //
                    lcd_line_count = 0;
                    M5.Lcd.clearDisplay();
                    M5.Lcd.setCursor(0, 0);
                    M5.Lcd.drawPng(ble_data_png.get(), ble_data_png_size);
                    //
                    M5.Lcd.println("DataRecved: PNG");
                    M5.Lcd.printf("  size: %d\n", ble_data_png_size);
                    lcd_line_count++;
                }
            }
        }

        end_disp();
    }
}

void check_psram()
{
    multi_heap_info_t info;
    heap_caps_get_info(&info, MALLOC_CAP_SPIRAM);
    M5.Lcd.printf("PSRAM: %d / %d\n", info.total_allocated_bytes, info.total_free_bytes);
    lcd_line_count++;
}
