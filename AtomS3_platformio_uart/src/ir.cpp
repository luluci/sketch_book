
#include "ir.hpp"

#include <IRremoteESP8266.h>
#include <IRsend.h>

const uint16_t kIrLed = 4;  // AtomS3 IR: G4
IRsend irsend(kIrLed);      // IR

void ir_init() {
    irsend.begin();
}

void ir_send() {
    // irsend.sendGeneric
}
