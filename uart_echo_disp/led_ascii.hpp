#pragma once

#undef __cplusplus
#define __cplusplus 201103L
#define _GLIBCXX_HAVE_STDINT_H 1

#include <cstdio>
#include <cstdint>
//#include <array>
//#include <initializer_list>

#include "led.hpp"

// LED表示パターン

//
using dot_t = uint8_t[dot_size];
using pattern_t = uint8_t[matrix_byte_size+2];
using pattern_ptr_t = uint8_t const *;

// バッファサイズ
static constexpr size_t led_ascii_buff_char_length = 100; // 文字数
static constexpr size_t led_ascii_buff_width = led_ascii_buff_char_length * dot_size;
static constexpr size_t led_ascii_buff_height = matrix_height;
static constexpr size_t led_ascii_buff_size = led_ascii_buff_width * led_ascii_buff_height;
extern uint8_t led_ascii_buff[led_ascii_buff_size+2];
extern uint8_t led_ascii_buff_count;

void buffer_to_ascii_led(uint8_t buff[], size_t count);

// 5x5パターン定義：空白定義
static constexpr uint8_t led_ptn_raw_margin[matrix_byte_size + 2] = {
	// size
	1,5,
	// ''
	OFF,OFF,OFF,OFF,OFF,
	OFF,OFF,OFF,OFF,OFF,
	OFF,OFF,OFF,OFF,OFF,
	OFF,OFF,OFF,OFF,OFF,
	OFF,OFF,OFF,OFF,OFF,
};

// 5x5パターン定義：未定義文字
static constexpr uint8_t led_ptn_raw_undef[matrix_byte_size+2] = {
    // size
	3,5,
    // ''
    BLU,BLU,BLU,OFF,OFF,
    BLU,BLU,BLU,OFF,OFF,
    BLU,BLU,BLU,OFF,OFF,
    BLU,BLU,BLU,OFF,OFF,
    BLU,BLU,BLU,OFF,OFF,
};

// 5x5パターン定義：記号
static constexpr uint8_t led_ptn_raw_x21[matrix_byte_size+2] = {
    // size
	3,5,
    // '!'
    BLU,OFF,OFF,OFF,OFF,
    BLU,OFF,OFF,OFF,OFF,
    BLU,OFF,OFF,OFF,OFF,
    OFF,OFF,OFF,OFF,OFF,
    BLU,OFF,OFF,OFF,OFF,
};
static constexpr uint8_t led_ptn_raw_x2B[matrix_byte_size+2] = {
    // size
    3,5,
    // '+'
    OFF,OFF,OFF,OFF,OFF,
    OFF,BLU,OFF,OFF,OFF,
    BLU,BLU,BLU,OFF,OFF,
    OFF,BLU,OFF,OFF,OFF,
    OFF,OFF,OFF,OFF,OFF,
};
static constexpr uint8_t led_ptn_raw_x2C[matrix_byte_size+2] = {
    // size
	2,5,
    // ','
    OFF,OFF,OFF,OFF,OFF,
    BLU,BLU,OFF,OFF,OFF,
    BLU,BLU,OFF,OFF,OFF,
    OFF,BLU,OFF,OFF,OFF,
    BLU,OFF,OFF,OFF,OFF,
};
static constexpr uint8_t led_ptn_raw_x2D[matrix_byte_size+2] = {
    // size
    2,5,
    // '-'
    OFF,OFF,OFF,OFF,OFF,
    OFF,OFF,OFF,OFF,OFF,
    BLU,BLU,OFF,OFF,OFF,
    OFF,OFF,OFF,OFF,OFF,
    OFF,OFF,OFF,OFF,OFF,
};
static constexpr uint8_t led_ptn_raw_x2E[matrix_byte_size+2] = {
    // size
    1,5,
    // '.'
    OFF,OFF,OFF,OFF,OFF,
    OFF,OFF,OFF,OFF,OFF,
    OFF,OFF,OFF,OFF,OFF,
    OFF,OFF,OFF,OFF,OFF,
    BLU,OFF,OFF,OFF,OFF,
};
static constexpr uint8_t led_ptn_raw_x2F[matrix_byte_size+2] = {
    // size
	3,5,
    // '/'
    OFF,OFF,BLU,OFF,OFF,
    OFF,OFF,BLU,OFF,OFF,
    OFF,BLU,OFF,OFF,OFF,
    BLU,OFF,OFF,OFF,OFF,
    BLU,OFF,OFF,OFF,OFF,
};
static constexpr uint8_t led_ptn_raw_x3A[matrix_byte_size+2] = {
    // size
    1,5,
    // ':'
    OFF,OFF,OFF,OFF,OFF,
    BLU,OFF,OFF,OFF,OFF,
    OFF,OFF,OFF,OFF,OFF,
    BLU,OFF,OFF,OFF,OFF,
    OFF,OFF,OFF,OFF,OFF,
};
static constexpr uint8_t led_ptn_raw_x3B[matrix_byte_size+2] = {
    // size
    1,5,
    // ';'
    OFF,OFF,OFF,OFF,OFF,
    BLU,OFF,OFF,OFF,OFF,
    OFF,OFF,OFF,OFF,OFF,
    BLU,OFF,OFF,OFF,OFF,
    BLU,OFF,OFF,OFF,OFF,
};
static constexpr uint8_t led_ptn_raw_x3F[matrix_byte_size+2] = {
    // size
    5,1,
    // '?'
    OFF,BLU,OFF,OFF,OFF,
    BLU,OFF,BLU,OFF,OFF,
    OFF,OFF,BLU,OFF,OFF,
    OFF,BLU,OFF,OFF,OFF,
    OFF,BLU,OFF,OFF,OFF,
};
static constexpr uint8_t led_ptn_raw_x5F[matrix_byte_size+2] = {
    // size
    2,5,
    // '_'
    OFF,OFF,OFF,OFF,OFF,
    OFF,OFF,OFF,OFF,OFF,
    OFF,OFF,OFF,OFF,OFF,
    OFF,OFF,OFF,OFF,OFF,
    BLU,BLU,OFF,OFF,OFF,
};

// 5x5パターン定義：記号1：'!'~'/'
using matrix_t = const uint8_t[matrix_byte_size+2];
static constexpr uint8_t const *led_ptn_raw_mark1[15] = {
    &led_ptn_raw_x21[0],
    &led_ptn_raw_undef[0],
    &led_ptn_raw_undef[0],
    &led_ptn_raw_undef[0],
    &led_ptn_raw_undef[0],
    &led_ptn_raw_undef[0],
    &led_ptn_raw_undef[0],
    &led_ptn_raw_undef[0],
    &led_ptn_raw_undef[0],
    &led_ptn_raw_undef[0],
    &led_ptn_raw_x2B[0],
    &led_ptn_raw_x2C[0],
    &led_ptn_raw_x2D[0],
    &led_ptn_raw_x2E[0],
    &led_ptn_raw_x2F[0],
};

// 5x5パターン定義：記号2：':'~'@'
static constexpr uint8_t const *led_ptn_raw_mark2[7] = {
    &led_ptn_raw_x3A[0],
    &led_ptn_raw_x3B[0],
    &led_ptn_raw_undef[0],
    &led_ptn_raw_undef[0],
    &led_ptn_raw_undef[0],
    &led_ptn_raw_x3F[0],
    &led_ptn_raw_undef[0],
};

// 5x5パターン定義：記号3：'['~'`'
static constexpr uint8_t const *led_ptn_raw_mark3[6] = {
    &led_ptn_raw_undef[0],
    &led_ptn_raw_undef[0],
    &led_ptn_raw_undef[0],
    &led_ptn_raw_undef[0],
    &led_ptn_raw_x5F[0],
    &led_ptn_raw_undef[0],
};

// 5x5パターン定義：'0'~'9'
static constexpr uint8_t led_ptn_raw_0_9[10][matrix_byte_size+2] = {
    {
        // size
		3,5,
        // '0'
        BLU,BLU,BLU,OFF,OFF,
        BLU,OFF,BLU,OFF,OFF,
        BLU,OFF,BLU,OFF,OFF,
        BLU,OFF,BLU,OFF,OFF,
        BLU,BLU,BLU,OFF,OFF,
    },
    {
        // size
		3,5,
        // '1'
        OFF,OFF,BLU,OFF,OFF,
        OFF,OFF,BLU,OFF,OFF,
        OFF,OFF,BLU,OFF,OFF,
        OFF,OFF,BLU,OFF,OFF,
        OFF,OFF,BLU,OFF,OFF,
    },
    {
        // size
		3,5,
        // '2'
        BLU,BLU,BLU,OFF,OFF,
        OFF,OFF,BLU,OFF,OFF,
        BLU,BLU,BLU,OFF,OFF,
        BLU,OFF,OFF,OFF,OFF,
        BLU,BLU,BLU,OFF,OFF,
    },
    {
        // size
		3,5,
        // '3'
        BLU,BLU,BLU,OFF,OFF,
        OFF,OFF,BLU,OFF,OFF,
        BLU,BLU,BLU,OFF,OFF,
        OFF,OFF,BLU,OFF,OFF,
        BLU,BLU,BLU,OFF,OFF,
    },
    {
        // size
		3,5,
        // '4'
        BLU,OFF,BLU,OFF,OFF,
        BLU,OFF,BLU,OFF,OFF,
        BLU,BLU,BLU,OFF,OFF,
        OFF,OFF,BLU,OFF,OFF,
        OFF,OFF,BLU,OFF,OFF,
    },
    {
        // size
		3,5,
        // '5'
        BLU,BLU,BLU,OFF,OFF,
        BLU,OFF,OFF,OFF,OFF,
        BLU,BLU,BLU,OFF,OFF,
        OFF,OFF,BLU,OFF,OFF,
        BLU,BLU,BLU,OFF,OFF,
    },
    {
        // size
		3,5,
        // '6'
        BLU,BLU,BLU,OFF,OFF,
        BLU,OFF,OFF,OFF,OFF,
        BLU,BLU,BLU,OFF,OFF,
        BLU,OFF,BLU,OFF,OFF,
        BLU,BLU,BLU,OFF,OFF,
    },
    {
        // size
		3,5,
        // '7'
        BLU,BLU,BLU,OFF,OFF,
        BLU,OFF,BLU,OFF,OFF,
        OFF,OFF,BLU,OFF,OFF,
        OFF,OFF,BLU,OFF,OFF,
        OFF,OFF,BLU,OFF,OFF,
    },
    {
        // size
		3,5,
        // '8'
        BLU,BLU,BLU,OFF,OFF,
        BLU,OFF,BLU,OFF,OFF,
        BLU,BLU,BLU,OFF,OFF,
        BLU,OFF,BLU,OFF,OFF,
        BLU,BLU,BLU,OFF,OFF,
    },
    {
        // size
		3,5,
        // '9'
        BLU,BLU,BLU,OFF,OFF,
        BLU,OFF,BLU,OFF,OFF,
        BLU,BLU,BLU,OFF,OFF,
        OFF,OFF,BLU,OFF,OFF,
        BLU,BLU,BLU,OFF,OFF,
    },
};
// 5x5パターン定義：'a'~'z'
static constexpr uint8_t led_ptn_raw_a_z[26][matrix_byte_size+2] = {
    {
        // size
        3,5,
        // 'A'
        OFF,BLU,OFF,OFF,OFF,
        BLU,OFF,BLU,OFF,OFF,
        BLU,OFF,BLU,OFF,OFF,
        BLU,BLU,BLU,OFF,OFF,
        BLU,OFF,BLU,OFF,OFF,
    },
    {
        // size
		3,5,
        // 'B'
        BLU,BLU,OFF,OFF,OFF,
        BLU,OFF,BLU,OFF,OFF,
        BLU,BLU,OFF,OFF,OFF,
        BLU,OFF,BLU,OFF,OFF,
        BLU,BLU,OFF,OFF,OFF,
    },
    {
        // size
		3,5,
        // 'C'
        OFF,BLU,BLU,OFF,OFF,
        BLU,OFF,OFF,OFF,OFF,
        BLU,OFF,OFF,OFF,OFF,
        BLU,OFF,OFF,OFF,OFF,
        OFF,BLU,BLU,OFF,OFF,
    },
    {
        // size
		3,5,
        // 'D'
        BLU,BLU,OFF,OFF,OFF,
        BLU,OFF,BLU,OFF,OFF,
        BLU,OFF,BLU,OFF,OFF,
        BLU,OFF,BLU,OFF,OFF,
        BLU,BLU,OFF,OFF,OFF,
    },
    {
        // size
		3,5,
        // 'E'
        BLU,BLU,BLU,OFF,OFF,
        BLU,OFF,OFF,OFF,OFF,
        BLU,BLU,BLU,OFF,OFF,
        BLU,OFF,OFF,OFF,OFF,
        BLU,BLU,BLU,OFF,OFF,
    },
    {
        // size
		3,5,
        // 'F'
        BLU,BLU,BLU,OFF,OFF,
        BLU,OFF,OFF,OFF,OFF,
        BLU,BLU,BLU,OFF,OFF,
        BLU,OFF,OFF,OFF,OFF,
        BLU,OFF,OFF,OFF,OFF,
    },
    {
        // size
        4,5,
        // 'G'
        BLU,BLU,BLU,BLU,OFF,
        BLU,OFF,OFF,OFF,OFF,
        BLU,OFF,BLU,BLU,OFF,
        BLU,OFF,OFF,BLU,OFF,
        BLU,BLU,BLU,BLU,OFF,
    },
    {
        // size
		3,5,
        // 'H'
        BLU,OFF,BLU,OFF,OFF,
        BLU,OFF,BLU,OFF,OFF,
        BLU,BLU,BLU,OFF,OFF,
        BLU,OFF,BLU,OFF,OFF,
        BLU,OFF,BLU,OFF,OFF,
    },
    {
        // size
        1,5,
        // 'i'
        BLU,OFF,OFF,OFF,OFF,
        OFF,OFF,OFF,OFF,OFF,
        BLU,OFF,OFF,OFF,OFF,
        BLU,OFF,OFF,OFF,OFF,
        BLU,OFF,OFF,OFF,OFF,
    },
    {
        // size
		3,5,
        // 'J'
        OFF,OFF,BLU,OFF,OFF,
        OFF,OFF,BLU,OFF,OFF,
        OFF,OFF,BLU,OFF,OFF,
        BLU,OFF,BLU,OFF,OFF,
        OFF,BLU,OFF,OFF,OFF,
    },
    {
        // size
        4,5,
        // 'K'
        BLU,OFF,OFF,BLU,OFF,
        BLU,OFF,BLU,OFF,OFF,
        BLU,BLU,OFF,OFF,OFF,
        BLU,OFF,BLU,OFF,OFF,
        BLU,OFF,OFF,BLU,OFF,
    },
    {
        // size
		3,5,
        // 'L'
        BLU,OFF,OFF,OFF,OFF,
        BLU,OFF,OFF,OFF,OFF,
        BLU,OFF,OFF,OFF,OFF,
        BLU,OFF,OFF,OFF,OFF,
        BLU,BLU,BLU,OFF,OFF,
    },
    {
        // size
        5,5,
        // 'M'
        BLU,OFF,OFF,OFF,BLU,
        BLU,BLU,OFF,BLU,BLU,
        BLU,OFF,BLU,OFF,BLU,
        BLU,OFF,OFF,OFF,BLU,
        BLU,OFF,OFF,OFF,BLU,
    },
    {
        // size
        5,5,
        // 'N'
        BLU,OFF,OFF,OFF,BLU,
        BLU,BLU,OFF,OFF,BLU,
        BLU,OFF,BLU,OFF,BLU,
        BLU,OFF,OFF,BLU,BLU,
        BLU,OFF,OFF,OFF,BLU,
    },
    {
        // size
        4,5,
        // 'O'
        OFF,BLU,BLU,OFF,OFF,
        BLU,OFF,OFF,BLU,OFF,
        BLU,OFF,OFF,BLU,OFF,
        BLU,OFF,OFF,BLU,OFF,
        OFF,BLU,BLU,OFF,OFF,
    },
    {
        // size
		3,5,
        // 'P'
        BLU,BLU,BLU,OFF,OFF,
        BLU,OFF,BLU,OFF,OFF,
        BLU,BLU,BLU,OFF,OFF,
        BLU,OFF,OFF,OFF,OFF,
        BLU,OFF,OFF,OFF,OFF,
    },
    {
        // size
		3,5,
        // 'q'
        OFF,OFF,OFF,OFF,OFF,
        BLU,BLU,BLU,OFF,OFF,
        BLU,OFF,BLU,OFF,OFF,
        BLU,BLU,BLU,OFF,OFF,
        OFF,OFF,BLU,OFF,OFF,
    },
    {
        // size
		3,5,
        // 'R'
        BLU,BLU,BLU,OFF,OFF,
        BLU,OFF,BLU,OFF,OFF,
        BLU,BLU,BLU,OFF,OFF,
        BLU,BLU,OFF,OFF,OFF,
        BLU,OFF,BLU,OFF,OFF,
    },
    {
        // size
		3,5,
        // 'S'
        OFF,BLU,BLU,OFF,OFF,
        BLU,OFF,OFF,OFF,OFF,
        OFF,BLU,OFF,OFF,OFF,
        OFF,OFF,BLU,OFF,OFF,
        BLU,BLU,OFF,OFF,OFF,
    },
    {
        // size
		3,5,
        // 'T'
        BLU,BLU,BLU,OFF,OFF,
        OFF,BLU,OFF,OFF,OFF,
        OFF,BLU,OFF,OFF,OFF,
        OFF,BLU,OFF,OFF,OFF,
        OFF,BLU,OFF,OFF,OFF,
    },
    {
        // size
		3,5,
        // 'U'
        BLU,OFF,BLU,OFF,OFF,
        BLU,OFF,BLU,OFF,OFF,
        BLU,OFF,BLU,OFF,OFF,
        BLU,OFF,BLU,OFF,OFF,
        OFF,BLU,OFF,OFF,OFF,
    },
    {
        // size
        5,5,
        // 'V'
        BLU,OFF,OFF,OFF,BLU,
        BLU,OFF,OFF,OFF,BLU,
        OFF,BLU,OFF,BLU,OFF,
        OFF,BLU,OFF,BLU,OFF,
        OFF,OFF,BLU,OFF,OFF,
    },
    {
        // size
        5,5,
        // 'W'
        BLU,OFF,BLU,OFF,BLU,
        BLU,OFF,BLU,OFF,BLU,
        BLU,OFF,BLU,OFF,BLU,
        BLU,OFF,BLU,OFF,BLU,
        OFF,BLU,OFF,BLU,OFF,
    },
    {
        // size
		3,5,
        // 'x'
        OFF,OFF,OFF,OFF,OFF,
        OFF,OFF,OFF,OFF,OFF,
        BLU,OFF,BLU,OFF,OFF,
        OFF,BLU,OFF,OFF,OFF,
        BLU,OFF,BLU,OFF,OFF,
    },
    {
        // size
		3,5,
        // 'Y'
        OFF,OFF,OFF,OFF,OFF,
        BLU,OFF,BLU,OFF,OFF,
        OFF,BLU,OFF,OFF,OFF,
        OFF,BLU,OFF,OFF,OFF,
        OFF,BLU,OFF,OFF,OFF,
    },
    {
        // size
        4,5,
        // 'Z'
        OFF,OFF,OFF,OFF,OFF,
        BLU,BLU,BLU,BLU,OFF,
        OFF,OFF,BLU,OFF,OFF,
        OFF,BLU,OFF,OFF,OFF,
        BLU,BLU,BLU,BLU,OFF,
    },
};

