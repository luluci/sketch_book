#pragma once

#include <cstddef>
#include <cstdint>

constexpr size_t buff_size = 1024;
extern size_t buff_pos;
extern uint8_t buff[buff_size];

// serial comm.
bool check_serial();
int read_serial();
void write_serial();

// utility
void hex2ascii(char buff[], uint8_t hex);
