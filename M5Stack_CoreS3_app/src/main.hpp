#pragma once

// clang-format off
#include <M5Unified.h>
// clang-format on

inline void M5_BEGIN(void)
{
    M5.begin();
}
inline void M5_BEGIN(m5::M5Unified::config_t &cfg)
{
    M5.begin(cfg);
}
inline void M5_UPDATE(void)
{
    M5.update();
}