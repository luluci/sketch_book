#pragma once

#include <lib_ble.hpp>
#include <ble/service/serial.hpp>
#include <ble/service/data_trans.hpp>

constexpr char const *SERVICE_UUID = "3e3388ee-4490-44ef-86c7-bb8c6dc73527";

// BLE Service 管理クラス
extern ble::service::serial ble_serial;
extern ble::service::data_trans ble_data_trans;

// data_trans取得データ
// PNGファイルバイナリデータ
extern ble::service::data_trans::psram_ptr_t ble_data_png;
extern size_t ble_data_png_size;
