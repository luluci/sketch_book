
#include "M5Atom.h"

#include <IRremoteESP8266.h>
#include <IRsend.h>

#include "serial.hpp"
#include "led_ascii.hpp"

// IR
const uint16_t kIrLed = 12;
IRsend irsend(kIrLed);

// LED
extern uint8_t const * const LED_MATRIX[];

void setup() {
  // put your setup code here, to run once:
  // begin: Serial=true, I2C=false, Display=true
  M5.begin(true, false, true);
  irsend.begin();
  //
  delay(50);
  // LED init
  M5.dis.setBrightness(10);
  M5.dis.setWidthHeight(matrix_width,matrix_height);
  M5.dis.clear();
  M5.dis.drawpix(0, 0x0000f0);
  // UART init
  //Serial.begin(115200);
}

void loop() {
  // 受信有無チェック
  if (check_serial()) {
    // LED初期化
    M5.dis.clear();
    M5.dis.setWidthHeight(matrix_width,matrix_height);
    M5.dis.drawpix(0, 0x0000f0);

    // 受信取得
    auto len = read_serial();
    // LED表示パターン変換
    buffer_to_ascii_led(buff, len);

    // 受信データをアニメーション表示
    M5.dis.setWidthHeight(led_ascii_buff_char_length, led_ascii_buff_height);
    M5.dis.animation(led_ascii_buff, 200, LED_DisPlay::kMoveLeft, led_ascii_buff_count-4);

    delay(500);

    // 受信データをそのまま送信
    write_serial();
  }

  delay(50);
}
