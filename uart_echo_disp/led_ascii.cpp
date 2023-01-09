
#include <cstdint>
#include <cstring>

#include "led.hpp"
#include "led_ascii.hpp"

uint8_t led_ascii_buff[led_ascii_buff_size+2];
uint8_t led_ascii_buff_count = 0;

// バッファ操作用定義
static constexpr size_t buff_row_size = led_ascii_buff_char_length * dot_size;
static constexpr size_t buff_base_idx_row_0 = buff_row_size * 0;
static constexpr size_t buff_base_idx_row_1 = buff_row_size * 1;
static constexpr size_t buff_base_idx_row_2 = buff_row_size * 2;
static constexpr size_t buff_base_idx_row_3 = buff_row_size * 3;
static constexpr size_t buff_base_idx_row_4 = buff_row_size * 4;

uint8_t const* get_ascii(uint8_t ch)
{
    if ('0' <= ch && ch <= '9')
    {
        return led_ptn_raw_0_9[ch - '0'];
    }
    else if ('a' <= ch && ch <= 'z')
    {
        return led_ptn_raw_a_z[ch - 'a'];
    }
    else if ('A' <= ch && ch <= 'Z')
    {
        return led_ptn_raw_a_z[ch - 'A'];
    }
    else if ('!' <= ch && ch <= '/')
    {
        return led_ptn_raw_mark1[ch - '!'];
    }
    else if (':' <= ch && ch <= '@')
    {
        return led_ptn_raw_mark2[ch - ':'];
    }
    else if ('[' <= ch && ch <= '`')
    {
        return led_ptn_raw_mark3[ch - '['];
    }
    else
    {
        return led_ptn_raw_undef;
    }
}

void ascii_to_buffer(uint8_t const ascii[matrix_byte_size+2], size_t pos)
{
    size_t buff_idx, matrix_idx;
    for (size_t col = 0; col < ascii[ptn_pos_w]; col++)
    {
        for (size_t row = 0; row < ascii[ptn_pos_h]; row++)
        {
            // index
            buff_idx = 2 + (row * led_ascii_buff_width) + ((pos + col) * dot_size);
            matrix_idx = 2 + (row * matrix_width * dot_size) + (col * dot_size);
            // copy
            memcpy((void *)&(led_ascii_buff[buff_idx]), (void *)&(ascii[matrix_idx]), dot_size);
        }
    }
}


void buffer_to_ascii_led(uint8_t buff[], size_t count)
{
    size_t pos = 0;

    for (size_t i=0; i < count; i++) {
        auto ascii = get_ascii(buff[i]);
        ascii_to_buffer(ascii, pos);
        pos += ascii[ptn_pos_w];
        //
        ascii_to_buffer(led_ptn_raw_margin, pos);
        pos += led_ptn_raw_margin[ptn_pos_w];
    }

    led_ascii_buff[ptn_pos_w] = pos;
    led_ascii_buff[ptn_pos_h] = matrix_height;

    led_ascii_buff_count = pos;
}

