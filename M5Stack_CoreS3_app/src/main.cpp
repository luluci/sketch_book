// clang-format off
#include "main.hpp"
// clang-format on

#include <cstdint>

#include "ir_recv.hpp"
#include "app.hpp"
#include "audio/audio.hpp"
#include "M5CoreS3Camera.hpp"

M5Canvas canvas(&M5.Lcd);

enum class event_t : uint8_t
{
    None,
    IR_recv,

    MAX
};

event_t get_event();
void action(event_t);

// LCDサイズ
int32_t lcd_w;
int32_t lcd_h;

void setup(void)
{
    M5_BEGIN();

    // カメラ初期化
    CoreS3Camera.begin();

    // speaker
    speaker.init();

    // // IR
    // ir_recv_init();

    // // 画面の初期設定
    // canvas.setTextColor(WHITE); // 文字色
    // // canvas.setTextDatum(middle_center); // テキストの座標基準指定
    // canvas.setTextSize(1);             // テキストサイズ
    // canvas.setFont(&fonts::Font4);     // フォント
    // lcd_w = M5.Lcd.width();            // 画面幅取得
    // lcd_h = M5.Lcd.height();           // 画面高さ取得
    // canvas.createSprite(lcd_w, lcd_h); // canvasサイズ（メモリ描画領域）設定（画面サイズに設定）

    M5.Display.setBrightness(60);

    app_init();
}

void loop(void)
{
    // event_t event;

    // M5_UPDATE();

    // {
    //     event = get_event();
    //     action(event);
    // }

    app_loop();

    // delay(50);
}

// event_t get_event()
// {
//     auto event = event_t::None;

//     auto ir_recv_result = ir_recv_check();
//     if (event == event_t::None)
//     {
//         if (ir_recv_result)
//         {
//             event = event_t::IR_recv;
//         }
//     }

//     return event;
// }

// void action(event_t event)
// {
//     switch (event)
//     {
//     case event_t::IR_recv:
//     {
//         canvas.fillScreen(BLACK);
//         canvas.setCursor(0, 0);
//         {
//             size_t idx = ir_recv_pool_pos;
//             if (idx == 0)
//             {
//                 idx = ir_recv_pool_size - 1;
//             }
//             else
//             {
//                 idx--;
//             }
//             auto &tgt = ir_recv_pool[idx];
//             canvas.printf("%s", tgt.c_str());
//             //
//             canvas.pushSprite(&M5.Lcd, 0, 0);
//         }
//     }
//     break;

//     default:
//         break;
//     }
// }