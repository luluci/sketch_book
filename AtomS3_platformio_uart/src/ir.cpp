
#include "ir.hpp"

#include <IRrecv.h>
#include <IRremoteESP8266.h>
#include <IRsend.h>
#include <ir_Hitachi.h>

const uint16_t kIrLed = 4;  // AtomS3 IR: G4
IRsend irsend(kIrLed);      // IR
IRHitachiAc ac(kIrLed);

uint8_t ir_buff[18] = {
    0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
    0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
};

void ir_init() {
    irsend.begin();
    irsend.calibrate();
    // ac.begin();
    // ac.on();
}

void ir_send(uint8_t const* buff, size_t buff_size) {
    // irsend.sendGeneric
    // irsend.sendMitsubishiAC(ir_buff, 18, 10);
    irsend.sendMitsubishiAC(buff, buff_size);

    // // Mitsubishi A/C
    // const uint16_t kMitsubishiAcHdrMark = 3400;
    // const uint16_t kMitsubishiAcHdrSpace = 1750;
    // const uint16_t kMitsubishiAcBitMark = 450;
    // const uint16_t kMitsubishiAcOneSpace = 1300;
    // const uint16_t kMitsubishiAcZeroSpace = 420;
    // const uint16_t kMitsubishiAcRptMark = 440;
    // const uint16_t kMitsubishiAcRptSpace = 15500;
    // const uint8_t kMitsubishiAcExtraTolerance = 5;

    // irsend.sendGeneric(kMitsubishiAcHdrMark, kMitsubishiAcHdrSpace,
    //                    kMitsubishiAcBitMark, kMitsubishiAcOneSpace,
    //                    kMitsubishiAcBitMark, kMitsubishiAcZeroSpace,
    //                    kMitsubishiAcRptMark, kMitsubishiAcRptSpace, buff,
    //                    buff_size, 38, false, 0, 50);

    // ac.send(0);
}
