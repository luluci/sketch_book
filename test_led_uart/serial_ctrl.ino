
#include "M5Atom.h"

constexpr size_t buff_size = 1024;
size_t buff_pos;
uint8_t buff[buff_size];

bool check_serial() {
  return Serial.available() > 0;
}

int read_serial() {
  int recv_len;
  buff_pos = 0;

  while (Serial.available() > 0) {
    recv_len = Serial.read(&buff[buff_pos], buff_size-buff_pos);
    buff_pos += recv_len;
  }

  return buff_pos;
}
