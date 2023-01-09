#pragma once

#undef __cplusplus
#define __cplusplus 201103L
#define _GLIBCXX_HAVE_STDINT_H 1

#include <cstdio>
#include <cstdint>
//#include <array>
//#include <initializer_list>

// LED表示パターン
// dot定義
#define RED 0xFF, 0x00, 0x00
#define GRN 0x00, 0xFF, 0x00
#define BLU 0x00, 0x00, 0xFF
#define OFF 0x00, 0x00, 0x00
// パターン配列サイズ
static constexpr size_t matrix_width = 5;
static constexpr size_t matrix_height = 5;
static constexpr size_t dot_size = 3;
static constexpr size_t matrix_byte_size = matrix_width * matrix_height * dot_size;
// パターン配列位置情報
static constexpr size_t ptn_pos_w = 0;
static constexpr size_t ptn_pos_h = 1;
static constexpr size_t ptn_pos_matrix = 2;
