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

#include <driver/periph_ctrl.h>
#include <driver/i2c.h>
#include <soc/soc.h>
#include <soc/i2c_reg.h>
#include <soc/i2c_struct.h>
#include <hal/i2c_ll.h>

#include "hal_def.h"

//-------------------------------------- HAL_LL (Missing Functions) ------------------------------------------------

static inline i2c_stretch_cause_t i2c_ll_stretch_cause(i2c_dev_t *hw)
{
#if CONFIG_IDF_TARGET_ESP32C3 || CONFIG_IDF_TARGET_ESP32S3
    return (i2c_stretch_cause_t)hw->sr.stretch_cause;
#elif CONFIG_IDF_TARGET_ESP32S2
    return hw->status_reg.stretch_cause;
#else
    return I2C_STRETCH_CAUSE_MAX;
#endif
}

static inline void i2c_ll_set_stretch(i2c_dev_t *hw, uint16_t time)
{
#ifndef CONFIG_IDF_TARGET_ESP32
    typeof(hw->scl_stretch_conf) scl_stretch_conf;
    scl_stretch_conf.val = 0;
    scl_stretch_conf.slave_scl_stretch_en = (time > 0);
    scl_stretch_conf.stretch_protect_num = time;
    scl_stretch_conf.slave_scl_stretch_clr = 1;
    scl_stretch_conf.slave_byte_ack_ctl_en = 1;
    hw->scl_stretch_conf.val = scl_stretch_conf.val;
    if (time > 0)
    {
        // enable interrupt
        hw->int_ena.val |= I2C_SLAVE_STRETCH_INT_ENA;
    }
    else
    {
        // disable interrupt
        hw->int_ena.val &= (~I2C_SLAVE_STRETCH_INT_ENA);
    }
#endif
}

static inline void i2c_ll_stretch_clr(i2c_dev_t *hw)
{
#ifndef CONFIG_IDF_TARGET_ESP32
    hw->scl_stretch_conf.slave_scl_stretch_clr = 1;
#endif
}

static inline void i2c_ll_set_ack(i2c_dev_t *hw, uint8_t ack)
{
#ifndef CONFIG_IDF_TARGET_ESP32
    hw->scl_stretch_conf.slave_byte_ack_lvl = ack;
#endif
}

static inline bool i2c_ll_slave_addressed(i2c_dev_t *hw)
{
#if CONFIG_IDF_TARGET_ESP32C3 || CONFIG_IDF_TARGET_ESP32S3
    return hw->sr.slave_addressed;
#else
    return hw->status_reg.slave_addressed;
#endif
}

static inline bool i2c_ll_slave_rw(i2c_dev_t *hw) // not exposed by hal_ll
{
#if CONFIG_IDF_TARGET_ESP32C3 || CONFIG_IDF_TARGET_ESP32S3
    return hw->sr.slave_rw;
#else
    return hw->status_reg.slave_rw;
#endif
}
