#pragma once

#include <ble/server.hpp>
#include <ble/defines.hpp>
#include <ble/service/serial.hpp>

constexpr char const *SERVICE_UUID = "3e3388ee-4490-44ef-86c7-bb8c6dc73527";
using ble_type = ble::server<ble::service::serial>;

extern ble_type ble_paper_s3;
// extern ble::service::serial ble_serial;
