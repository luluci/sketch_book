// clang-format off
#include <epdiy.h>
#include <M5Unified.h>
// clang-format on

#include "ble_paper_s3.hpp"

void app();
void check_ble_pairing();
void check_event();

void setup()
{
    // m5::M5Unified::config_t cfg
    M5.begin();

    M5.Lcd.setTextSize(6);
    M5.Lcd.setCursor(5, 5);
    M5.Lcd.println("Init Start.");

    M5.Lcd.println("Init BLE.");
    ble_paper_s3.setup("M5PaperS3", ble::service::serial{});

    M5.Lcd.println("Init Fin.");
}

void loop()
{
    M5.update();

    app();

    delay(100);
}

enum class app_state
{
    InitWait,
    WaitBlePairing,
    Running,
};
app_state state = app_state::InitWait;
size_t timer = 0;

void app()
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
            ble_paper_s3.start();
            //
            M5.Lcd.startWrite();
            M5.Lcd.clearDisplay();
            M5.Lcd.setCursor(5, 5);
            M5.Lcd.println("BLE Start. Wait Pairing.");
            M5.Lcd.print("PIN: ");
            M5.Lcd.println(ble_paper_s3.get_device_addr_str());
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
}

void check_ble_pairing()
{
    if (ble_paper_s3.is_connected())
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
    ble::service::serial &serial = ble_paper_s3.get_service<0>();
    auto event = serial.get_event();

    switch (event)
    {
    case (uint16_t)ble::service::serial_event::HasRecieve:
        // 受信処理
        M5.Lcd.startWrite();
        M5.Lcd.clearDisplay();
        M5.Lcd.setCursor(5, 5);
        for (size_t i = 0; i < 10; i++)
        {
            M5.Lcd.println(serial.rx_buffer[i].c_str());
        }
        M5.Lcd.endWrite();
        break;
    }
}
