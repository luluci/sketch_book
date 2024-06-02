#pragma once

#include <cstddef>
#include <cstdint>

void ir_init();
void ir_send(uint8_t const* buff, size_t buff_size);
