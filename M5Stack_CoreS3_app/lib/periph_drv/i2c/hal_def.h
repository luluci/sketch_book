// 本ソースは framework-arduinoespressif32\cores\esp32\esp32-hal-i2c-slave.c を流用・改変している。
// Licenseは下記の通り

// Copyright 2015-2021 Espressif Systems (Shanghai) PTE LTD
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

#pragma once

typedef enum
{
    I2C_ACK = 0,
    I2C_NACK = 1,
} i2c_ack_lvl;

//-------------------------------------- HAL_LL (Missing Functions) ------------------------------------------------
typedef enum
{
    I2C_STRETCH_CAUSE_MASTER_READ,
    I2C_STRETCH_CAUSE_TX_FIFO_EMPTY,
    I2C_STRETCH_CAUSE_RX_FIFO_FULL,
    I2C_STRETCH_CAUSE_MAX
} i2c_stretch_cause_t;
