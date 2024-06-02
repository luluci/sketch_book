#pragma once

#include <cstddef>
#include <cstdint>

namespace serial {

enum tool_mode : uint8_t
{
    DISP_SERIAL,  // シリアル受信内容を表示
    DISP_I2C,     // I2C内容を表示
};

struct protocol_frame
{
    uint8_t protocol_code;
    tool_mode mode;
    uint8_t i2c_addr;
    uint8_t i2c_reg;
    uint8_t reserve[92];
};
constexpr size_t frame_size = sizeof(protocol_frame);
union buffer
{
    protocol_frame frame;
    uint8_t bytes[frame_size];
};
constexpr size_t buffer_size = sizeof(buffer);

extern size_t buff_pos;
extern buffer buff;

}  // namespace serial

// constexpr size_t buff_size = 1024;
// extern size_t buff_pos;
// extern uint8_t buff[buff_size];

// serial comm.
bool check_serial();
int read_serial();
void write_serial();

// utility
void hex2ascii(char buff[], uint8_t hex);
