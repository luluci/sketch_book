#include "ble_paper_s3.hpp"

ble::service::serial ble_serial;
ble::service::data_trans ble_data_trans;

ble::service::data_trans::psram_ptr_t ble_data_png;
size_t ble_data_png_size;
