#pragma once

#include "../defines.hpp"

#include <BLEServer.h>
#include <memory>

namespace ble::service
{
    class service_base
    {
    protected:
        // Service UUID
        char const *uuid_;
        // Service
        BLEService *service_ = nullptr;

    public:
        service_base(char const *uuid);
        ~service_base();

        bool setup(BLEServer *server);

        void start() {}
        void stop() {}

        char const *get_uuid() const { return uuid_; }

    private:
        service_state state_;
    };
}
