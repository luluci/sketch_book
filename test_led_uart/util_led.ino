
#undef __cplusplus
#define __cplusplus 201103L

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
// 
static constexpr size_t matrix_width = 5;
static constexpr size_t matrix_height = 5;
static constexpr size_t dot_size = 3;
static constexpr size_t matrix_byte_size = matrix_width * matrix_height * dot_size;
//
using dot_t = uint8_t[dot_size];
using pattern_t = uint8_t[matrix_byte_size+2];
using pattern_ptr_t = uint8_t const *;
// Lo 4bit パターン定義：0x00~0x0F
static constexpr uint8_t led_ptn_raw[0x10][matrix_byte_size+2] = {
    // 0x00
    {
        // width, height
        5, 5, 
        // dot 5x5
        OFF,OFF,BLU,BLU,BLU,
        OFF,OFF,BLU,OFF,BLU,
        OFF,OFF,BLU,OFF,BLU,
        OFF,OFF,BLU,OFF,BLU,
        OFF,OFF,BLU,BLU,BLU,
    },
    // 0x01
    {
        // width, height
        5, 5, 
        // dot 5x5
        OFF,OFF,OFF,OFF,BLU,
        OFF,OFF,OFF,OFF,BLU,
        OFF,OFF,OFF,OFF,BLU,
        OFF,OFF,OFF,OFF,BLU,
        OFF,OFF,OFF,OFF,BLU,
    },
    // 0x02
    {
        // width, height
        5, 5, 
        // dot 5x5
        OFF,OFF,BLU,BLU,BLU,
        OFF,OFF,OFF,OFF,BLU,
        OFF,OFF,BLU,BLU,BLU,
        OFF,OFF,BLU,OFF,OFF,
        OFF,OFF,BLU,BLU,BLU,
    },
    // 0x03
    {
        // width, height
        5, 5, 
        // dot 5x5
        OFF,OFF,BLU,BLU,BLU,
        OFF,OFF,OFF,OFF,BLU,
        OFF,OFF,BLU,BLU,BLU,
        OFF,OFF,OFF,OFF,BLU,
        OFF,OFF,BLU,BLU,BLU,
    },
    // 0x04
    {
        // width, height
        5, 5, 
        // dot 5x5
        OFF,OFF,BLU,OFF,BLU,
        OFF,OFF,BLU,OFF,BLU,
        OFF,OFF,BLU,BLU,BLU,
        OFF,OFF,OFF,OFF,BLU,
        OFF,OFF,OFF,OFF,BLU,
    },
    // 0x05
    {
        // width, height
        5, 5, 
        // dot 5x5
        OFF,OFF,BLU,BLU,BLU,
        OFF,OFF,BLU,OFF,OFF,
        OFF,OFF,BLU,BLU,BLU,
        OFF,OFF,OFF,OFF,BLU,
        OFF,OFF,BLU,BLU,BLU,
    },
    // 0x06
    {
        // width, height
        5, 5, 
        // dot 5x5
        OFF,OFF,BLU,BLU,BLU,
        OFF,OFF,BLU,OFF,OFF,
        OFF,OFF,BLU,BLU,BLU,
        OFF,OFF,BLU,OFF,BLU,
        OFF,OFF,BLU,BLU,BLU,
    },
    // 0x07
    {
        // width, height
        5, 5, 
        // dot 5x5
        OFF,OFF,BLU,BLU,BLU,
        OFF,OFF,BLU,OFF,BLU,
        OFF,OFF,OFF,OFF,BLU,
        OFF,OFF,OFF,OFF,BLU,
        OFF,OFF,OFF,OFF,BLU,
    },
    // 0x08
    {
        // width, height
        5, 5, 
        // dot 5x5
        OFF,OFF,BLU,BLU,BLU,
        OFF,OFF,BLU,OFF,BLU,
        OFF,OFF,BLU,BLU,BLU,
        OFF,OFF,BLU,OFF,BLU,
        OFF,OFF,BLU,BLU,BLU,
    },
    // 0x09
    {
        // width, height
        5, 5, 
        // dot 5x5
        OFF,OFF,BLU,BLU,BLU,
        OFF,OFF,BLU,OFF,BLU,
        OFF,OFF,BLU,BLU,BLU,
        OFF,OFF,OFF,OFF,BLU,
        OFF,OFF,BLU,BLU,BLU,
    },
    // 0x0A
    {
        // width, height
        5, 5, 
        // dot 5x5
        OFF,OFF,OFF,BLU,OFF,
        OFF,OFF,BLU,OFF,BLU,
        OFF,OFF,BLU,OFF,BLU,
        OFF,OFF,BLU,BLU,BLU,
        OFF,OFF,BLU,OFF,BLU,
    },
    // 0x0B
    {
        // width, height
        5, 5, 
        // dot 5x5
        OFF,OFF,BLU,BLU,OFF,
        OFF,OFF,BLU,OFF,BLU,
        OFF,OFF,BLU,BLU,OFF,
        OFF,OFF,BLU,OFF,BLU,
        OFF,OFF,BLU,BLU,OFF,
    },
    // 0x0C
    {
        // width, height
        5, 5, 
        // dot 5x5
        OFF,OFF,OFF,BLU,BLU,
        OFF,OFF,BLU,OFF,OFF,
        OFF,OFF,BLU,OFF,OFF,
        OFF,OFF,BLU,OFF,OFF,
        OFF,OFF,OFF,BLU,BLU,
    },
    // 0x0D
    {
        // width, height
        5, 5, 
        // dot 5x5
        OFF,OFF,BLU,BLU,OFF,
        OFF,OFF,BLU,OFF,BLU,
        OFF,OFF,BLU,OFF,BLU,
        OFF,OFF,BLU,OFF,BLU,
        OFF,OFF,BLU,BLU,OFF,
    },
    // 0x0E
    {
        // width, height
        5, 5, 
        // dot 5x5
        OFF,OFF,BLU,BLU,BLU,
        OFF,OFF,BLU,OFF,OFF,
        OFF,OFF,BLU,BLU,BLU,
        OFF,OFF,BLU,OFF,OFF,
        OFF,OFF,BLU,BLU,BLU,
    },
    // 0x0F
    {
        // width, height
        5, 5, 
        // dot 5x5
        OFF,OFF,BLU,BLU,BLU,
        OFF,OFF,BLU,OFF,OFF,
        OFF,OFF,BLU,BLU,BLU,
        OFF,OFF,BLU,OFF,OFF,
        OFF,OFF,BLU,OFF,OFF,
    },
};

template <size_t Value>
struct LED_pattern
{
    //
    static constexpr size_t H = (Value & 0xF0) >> 4;
    static constexpr size_t L = (Value & 0x0F);
    //
    static constexpr size_t w = 5;
    static constexpr size_t h = 5;
    static constexpr size_t dot_size = 3;
    static constexpr size_t size = w * h * dot_size;
    uint8_t pattern[size + 2];

    LED_pattern()
    {
        for (size_t i=0; i<sizeof(pattern); i++) pattern[i] = led_ptn_raw[L][i];

        // high pattern
        if (H >= 5) high_pattern<5>(BLU);
        if (H >= 4) high_pattern<4>(BLU);
        if (H >= 3) high_pattern<3>(BLU);
        if (H >= 2) high_pattern<2>(BLU);
        if (H >= 1) high_pattern<1>(BLU);
    }

    template<size_t row>
    void high_pattern(uint8_t r, uint8_t g, uint8_t b) {
        constexpr size_t pos = 2 + (h - row) * (h * 3);
        pattern[pos + 0] = r;
        pattern[pos + 1] = g;
        pattern[pos + 2] = b;
    }
};

struct LED_pattern_ex
{
    //
    uint8_t pattern[matrix_byte_size + 2];

    LED_pattern_ex(uint8_t const*const buff)
    {
        for (size_t i=0; i<sizeof(pattern); i++) pattern[i] = buff[i];
    }
};


// 0x00-0x0F
auto LED_00 = LED_pattern<0x00>();
auto LED_01 = LED_pattern<0x01>();
auto LED_02 = LED_pattern<0x02>();
auto LED_03 = LED_pattern<0x03>();
auto LED_04 = LED_pattern<0x04>();
auto LED_05 = LED_pattern<0x05>();
auto LED_06 = LED_pattern<0x06>();
auto LED_07 = LED_pattern<0x07>();
auto LED_08 = LED_pattern<0x08>();
auto LED_09 = LED_pattern<0x09>();
auto LED_0A = LED_pattern<0x0A>();
auto LED_0B = LED_pattern<0x0B>();
auto LED_0C = LED_pattern<0x0C>();
auto LED_0D = LED_pattern<0x0D>();
auto LED_0E = LED_pattern<0x0E>();
auto LED_0F = LED_pattern<0x0F>();
// 0x10-0x1F
auto LED_10 = LED_pattern<0x10>();
auto LED_11 = LED_pattern<0x11>();
auto LED_12 = LED_pattern<0x12>();
auto LED_13 = LED_pattern<0x13>();
auto LED_14 = LED_pattern<0x14>();
auto LED_15 = LED_pattern<0x15>();
auto LED_16 = LED_pattern<0x16>();
auto LED_17 = LED_pattern<0x17>();
auto LED_18 = LED_pattern<0x18>();
auto LED_19 = LED_pattern<0x19>();
auto LED_1A = LED_pattern<0x1A>();
auto LED_1B = LED_pattern<0x1B>();
auto LED_1C = LED_pattern<0x1C>();
auto LED_1D = LED_pattern<0x1D>();
auto LED_1E = LED_pattern<0x1E>();
auto LED_1F = LED_pattern<0x1F>();
// 0x20-0x2F
auto LED_20 = LED_pattern<0x20>();
auto LED_21 = LED_pattern<0x21>();
auto LED_22 = LED_pattern<0x22>();
auto LED_23 = LED_pattern<0x23>();
auto LED_24 = LED_pattern<0x24>();
auto LED_25 = LED_pattern<0x25>();
auto LED_26 = LED_pattern<0x26>();
auto LED_27 = LED_pattern<0x27>();
auto LED_28 = LED_pattern<0x28>();
auto LED_29 = LED_pattern<0x29>();
auto LED_2A = LED_pattern<0x2A>();
auto LED_2B = LED_pattern<0x2B>();
auto LED_2C = LED_pattern<0x2C>();
auto LED_2D = LED_pattern<0x2D>();
auto LED_2E = LED_pattern<0x2E>();
auto LED_2F = LED_pattern<0x2F>();
// 0x30-0x3F
auto LED_30 = LED_pattern<0x30>();
auto LED_31 = LED_pattern<0x31>();
auto LED_32 = LED_pattern<0x32>();
auto LED_33 = LED_pattern<0x33>();
auto LED_34 = LED_pattern<0x34>();
auto LED_35 = LED_pattern<0x35>();
auto LED_36 = LED_pattern<0x36>();
auto LED_37 = LED_pattern<0x37>();
auto LED_38 = LED_pattern<0x38>();
auto LED_39 = LED_pattern<0x39>();
auto LED_3A = LED_pattern<0x3A>();
auto LED_3B = LED_pattern<0x3B>();
auto LED_3C = LED_pattern<0x3C>();
auto LED_3D = LED_pattern<0x3D>();
auto LED_3E = LED_pattern<0x3E>();
auto LED_3F = LED_pattern<0x3F>();
// 0x40-0x4F
auto LED_40 = LED_pattern<0x40>();
auto LED_41 = LED_pattern<0x41>();
auto LED_42 = LED_pattern<0x42>();
auto LED_43 = LED_pattern<0x43>();
auto LED_44 = LED_pattern<0x44>();
auto LED_45 = LED_pattern<0x45>();
auto LED_46 = LED_pattern<0x46>();
auto LED_47 = LED_pattern<0x47>();
auto LED_48 = LED_pattern<0x48>();
auto LED_49 = LED_pattern<0x49>();
auto LED_4A = LED_pattern<0x4A>();
auto LED_4B = LED_pattern<0x4B>();
auto LED_4C = LED_pattern<0x4C>();
auto LED_4D = LED_pattern<0x4D>();
auto LED_4E = LED_pattern<0x4E>();
auto LED_4F = LED_pattern<0x4F>();
// 0x50-0x5F
auto LED_50 = LED_pattern<0x50>();
auto LED_51 = LED_pattern<0x51>();
auto LED_52 = LED_pattern<0x52>();
auto LED_53 = LED_pattern<0x53>();
auto LED_54 = LED_pattern<0x54>();
auto LED_55 = LED_pattern<0x55>();
auto LED_56 = LED_pattern<0x56>();
auto LED_57 = LED_pattern<0x57>();
auto LED_58 = LED_pattern<0x58>();
auto LED_59 = LED_pattern<0x59>();
auto LED_5A = LED_pattern<0x5A>();
auto LED_5B = LED_pattern<0x5B>();
auto LED_5C = LED_pattern<0x5C>();
auto LED_5D = LED_pattern<0x5D>();
auto LED_5E = LED_pattern<0x5E>();
auto LED_5F = LED_pattern<0x5F>();
// ex
uint8_t ex_ptn[matrix_byte_size+2] = {
    // width, height
    5, 5, 
    // dot 5x5
    OFF,OFF,OFF,OFF,OFF,
    OFF,OFF,OFF,OFF,OFF,
    OFF,OFF,OFF,OFF,OFF,
    OFF,OFF,OFF,OFF,OFF,
    OFF,OFF,OFF,OFF,OFF,
};
auto LED_EX = LED_pattern_ex(ex_ptn);

uint8_t const * const LED_MATRIX[] = {
    // 0x00-0x0F
    LED_00.pattern,
    LED_01.pattern,
    LED_02.pattern,
    LED_03.pattern,
    LED_04.pattern,
    LED_05.pattern,
    LED_06.pattern,
    LED_07.pattern,
    LED_08.pattern,
    LED_09.pattern,
    LED_0A.pattern,
    LED_0B.pattern,
    LED_0C.pattern,
    LED_0D.pattern,
    LED_0E.pattern,
    LED_0F.pattern,
    // 0x10-0x1F
    LED_10.pattern,
    LED_11.pattern,
    LED_12.pattern,
    LED_13.pattern,
    LED_14.pattern,
    LED_15.pattern,
    LED_16.pattern,
    LED_17.pattern,
    LED_18.pattern,
    LED_19.pattern,
    LED_1A.pattern,
    LED_1B.pattern,
    LED_1C.pattern,
    LED_1D.pattern,
    LED_1E.pattern,
    LED_1F.pattern,
    // 0x20-0x2F
    LED_20.pattern,
    LED_21.pattern,
    LED_22.pattern,
    LED_23.pattern,
    LED_24.pattern,
    LED_25.pattern,
    LED_26.pattern,
    LED_27.pattern,
    LED_28.pattern,
    LED_29.pattern,
    LED_2A.pattern,
    LED_2B.pattern,
    LED_2C.pattern,
    LED_2D.pattern,
    LED_2E.pattern,
    LED_2F.pattern,
    // 0x30-0x3F
    LED_30.pattern,
    LED_31.pattern,
    LED_32.pattern,
    LED_33.pattern,
    LED_34.pattern,
    LED_35.pattern,
    LED_36.pattern,
    LED_37.pattern,
    LED_38.pattern,
    LED_39.pattern,
    LED_3A.pattern,
    LED_3B.pattern,
    LED_3C.pattern,
    LED_3D.pattern,
    LED_3E.pattern,
    LED_3F.pattern,
    // 0x40-0x4F
    LED_40.pattern,
    LED_41.pattern,
    LED_42.pattern,
    LED_43.pattern,
    LED_44.pattern,
    LED_45.pattern,
    LED_46.pattern,
    LED_47.pattern,
    LED_48.pattern,
    LED_49.pattern,
    LED_4A.pattern,
    LED_4B.pattern,
    LED_4C.pattern,
    LED_4D.pattern,
    LED_4E.pattern,
    LED_4F.pattern,
    // 0x50-0x5F
    LED_50.pattern,
    LED_51.pattern,
    LED_52.pattern,
    LED_53.pattern,
    LED_54.pattern,
    LED_55.pattern,
    LED_56.pattern,
    LED_57.pattern,
    LED_58.pattern,
    LED_59.pattern,
    LED_5A.pattern,
    LED_5B.pattern,
    LED_5C.pattern,
    LED_5D.pattern,
    LED_5E.pattern,
    LED_5F.pattern,

    // ex
    LED_EX.pattern,
};
