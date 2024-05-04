#include "speaker.hpp"

// clang-format off
#include <M5Unified.h>
// clang-format on

#include "wave/wave.hpp"

namespace audio
{

    speaker::speaker()
    {
    }

    void speaker::init()
    {
        auto config = M5.Speaker.config();
        config.sample_rate = 44100;
        M5.Speaker.config(config);
        M5.Speaker.setVolume(200);
        M5.Speaker.begin();
    }
    void speaker::play()
    {
        M5.Speaker.playRaw((int16_t const *)wave::wave_data, wave::wave_data_size / 2, 44100, false);
    }
}
