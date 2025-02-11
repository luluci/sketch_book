#pragma once

#include "../defines.hpp"
#include "service_base.hpp"

#include <BLEServer.h>
#include <BLEUtils.h>
#include <BLE2902.h>

#include <memory>
#include <array>
#include <string>

namespace ble::service
{
    enum class serial_event : uint16_t
    {
        None = 0,
        HasRecieve = 0x01,
        HasRead = 0x02,

        MAX
    };

    class serial : public service_base, public BLECharacteristicCallbacks
    {
    public:
        uint16_t event;
        uint16_t get_event()
        {
            auto result = event;
            event = static_cast<uint16_t>(serial_event::None);
            return result;
        }

    private:
        static constexpr char const *const NORDIC_SERVICE_UUID = "6e400001-b5a3-f393-e0a9-e50e24dcca9e";
        static constexpr char const *const NORDIC_RX_UUID = "6e400002-b5a3-f393-e0a9-e50e24dcca9e";
        static constexpr char const *const NORDIC_TX_UUID = "6e400003-b5a3-f393-e0a9-e50e24dcca9e";
        // Serial Characteristics
        BLECharacteristic *tx_characteristic;
        BLECharacteristic *rx_characteristic;
        BLE2902 tx_descriptor;
        BLE2902 rx_descriptor;

    public:
        // 受信バッファ
        static constexpr size_t rx_buffer_size = 10;
        std::array<std::string, rx_buffer_size> rx_buffer;
        size_t rx_buffer_index = 0;

    public:
        serial();
        ~serial();

        bool setup(BLEServer *server) override;

        void onWrite(BLECharacteristic *pCharacteristic) override;
        void onRead(BLECharacteristic *pCharacteristic) override;

        void start() override;
        void stop() override;

    private:
        service_state state_;
    };
}
