
#include "serial.hpp"

#include <M5Unified.h>

#define SERIAL_P Serial

size_t buff_pos;
uint8_t buff[buff_size];

// prototype
char hex2ascii_impl(uint8_t hex);

bool check_serial() {
    return SERIAL_P.available() > 0;
}

int read_serial() {
    int recv_len;
    buff_pos = 0;

    while (SERIAL_P.available() > 0) {
        recv_len = SERIAL_P.read(&buff[buff_pos], buff_size - buff_pos);
        buff_pos += recv_len;
    }

    return buff_pos;
}

void write_serial() {
    SERIAL_P.write(buff, buff_pos);
}

/// @brief HEXデータをHEX文字列に変換してbuffに格納する
/// @param buff
/// @param hex
void hex2ascii(char buff[], uint8_t hex) {
    uint8_t hi = (hex >> 4) & 0x0F;
    uint8_t lo = hex & 0x0F;
    buff[0] = hex2ascii_impl(hi);
    buff[1] = hex2ascii_impl(lo);
}

char hex2ascii_impl(uint8_t hex) {
    if (0 <= hex && hex <= 9) {
        return hex + '0';
    } else if (0x0A <= hex && hex <= 0x0F) {
        return hex + ('A' - 0x0A);
    } else {
        return '?';
    }
}
