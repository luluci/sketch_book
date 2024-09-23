#pragma once

#include <M5Unified.h>

#include <IRrecv.h>
#include <IRremoteESP8266.h>
#include <IRutils.h>

#include <cstdint>
#include <cstddef>

constexpr uint16_t ir_recv_buffer_size = 1024;
constexpr uint16_t ir_recv_timeout = 50;

extern IRrecv ir_recv;
extern decode_results ir_recv_results;

constexpr size_t ir_recv_buff_size = 256;
constexpr size_t ir_recv_pool_size = 10;

// struct ir_recv_info_t
// {
//     uint8_t buff[ir_recv_buff_size];
// };

extern String ir_recv_pool[ir_recv_pool_size];
extern size_t ir_recv_pool_pos;
extern size_t ir_recv_pool_begin;

void ir_recv_init();
bool ir_recv_check();