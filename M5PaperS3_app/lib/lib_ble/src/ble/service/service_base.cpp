#include "service_base.hpp"
#include <BLEServer.h>

namespace ble::service
{
    service_base::service_base(char const *uuid) : uuid_(uuid)
    {
    }

    service_base::~service_base()
    {
    }

    bool service_base::setup(BLEServer *server)
    {
        // Service
        service_ = server->createService(uuid_);

        return service_ != nullptr;
    }
    void service_base::start()
    {
        if (service_ != nullptr)
        {
            service_->start();
        }
    }
    void service_base::stop()
    {
        if (service_ != nullptr)
        {
            service_->stop();
        }
    }
}