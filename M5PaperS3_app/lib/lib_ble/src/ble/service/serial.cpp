#include "serial.hpp"

namespace ble::service
{
    serial::serial() : service_base(NORDIC_SERVICE_UUID)
    {
    }

    serial::~serial()
    {
    }

    bool serial::setup(BLEServer *server)
    {
        // Service作成
        auto result = service_base::setup(server);
        if (!result)
        {
            return false;
        }
        //
        tx_characteristic = service_->createCharacteristic(NORDIC_TX_UUID, BLECharacteristic::PROPERTY_NOTIFY);
        rx_characteristic = service_->createCharacteristic(NORDIC_RX_UUID, BLECharacteristic::PROPERTY_WRITE);
        tx_characteristic->addDescriptor(&tx_descriptor);
        rx_characteristic->addDescriptor(&rx_descriptor);
        tx_characteristic->setReadProperty(true);
        rx_characteristic->setWriteProperty(true);

        tx_characteristic->setWriteNoResponseProperty(true);
        rx_characteristic->setWriteNoResponseProperty(true);

        tx_characteristic->setCallbacks(this);
        rx_characteristic->setCallbacks(this);

        return true;
    }
    void serial::onWrite(BLECharacteristic *pCharacteristic)
    {
        //
        // if (state_ != service_state::Active)
        //     return;
        //
        // if (pCharacteristic->getUUID().toString() == NORDIC_RX_UUID)
        {
            //
            if (rx_buffer_index < rx_buffer_size)
            {
                rx_buffer[rx_buffer_index] = pCharacteristic->getValue();
                rx_buffer_index++;
                event |= (uint16_t)serial_event::HasRecieve;
            }
        }
    }
    void serial::onRead(BLECharacteristic *pCharacteristic)
    {
        event |= (uint16_t)serial_event::HasRead;
    }

    void serial::start()
    {
        //
        service_base::start();
        //
        state_ = service_state::Active;
        event = static_cast<uint16_t>(serial_event::None);
    }
    void serial::stop()
    {
        //
        service_base::stop();
        //
        state_ = service_state::Stop;
    }
}
