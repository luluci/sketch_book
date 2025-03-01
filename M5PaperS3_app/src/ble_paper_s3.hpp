#pragma once

#include <lib_ble.hpp>
#include <ble/service/serial.hpp>
#include <ble/service/data_trans.hpp>

constexpr char const *SERVICE_UUID = "3e3388ee-4490-44ef-86c7-bb8c6dc73527";

extern ble::service::serial ble_serial;
extern ble::service::data_trans ble_data_trans;
