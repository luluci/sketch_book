#pragma once

#include <cstdint>

namespace ble
{

    enum class server_state : uint8_t
    {
        Init,        // 初期状態
        Advertising, // アドバタイジング中
        Connected,   // 接続中
    };
    enum class service_state : uint8_t
    {
        Init,
        Active,
        Stop,
    };
}
