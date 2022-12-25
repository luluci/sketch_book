
#include "M5Atom.h"

extern uint8_t const * const LED_MATRIX[];

void setup() {
  // put your setup code here, to run once:
  // begin: Serial=true, I2C=false, Display=true
  M5.begin(true, false, true);
  //
  delay(50);
  // LED init
  M5.dis.setBrightness(10);
  M5.dis.setWidthHeight(5,5);
  M5.dis.clear();
  M5.dis.drawpix(0, 0x0000f0);
  // UART init
  //Serial.begin(115200);
}

void loop() {
  int buff;
  // put your main code here, to run repeatedly:
  // buff = Serial.read();
  // if (buff != -1) {
  //   switch (buff) {
  //   case 'A':
  //     M5.dis.drawpix(0, 0xff0000);
  //     break;
  //   default:
  //     M5.dis.drawpix(0, 0xfff000);
  //     break;
  //   }
  // }
  if (check_serial()) {
    auto len = read_serial();
    M5.dis.displaybuff((uint8_t*)LED_MATRIX[len]);
    //M5.dis.displaybuff((uint8_t*)LED_MATRIX[0x60]);
  }

  delay(50);
}
