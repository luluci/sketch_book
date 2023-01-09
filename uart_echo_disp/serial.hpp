#pragma once

#include <cstdint>

constexpr size_t buff_size = 1024;
extern size_t buff_pos;
extern uint8_t buff[buff_size];

bool check_serial();
int read_serial();
void write_serial();
