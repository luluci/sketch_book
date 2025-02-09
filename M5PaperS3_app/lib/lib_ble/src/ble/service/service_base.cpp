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

        return true;
    }
}