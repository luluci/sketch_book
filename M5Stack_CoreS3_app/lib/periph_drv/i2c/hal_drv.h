#pragma once

#include <driver/periph_ctrl.h>
#include <driver/i2c.h>
#include <soc/soc.h>
#include <soc/i2c_reg.h>
#include <soc/i2c_struct.h>

typedef struct i2c_setting
{
    i2c_dev_t *dev;
    periph_module_t module_no;
    periph_interrput_t intr_no;
    gpio_num_t pin_scl;
    gpio_num_t pin_sda;
    uint32_t signal_scl;
    uint32_t signal_sda;
    uint16_t slave_id;
    uint32_t frequency;
    //
    intr_handler_t handler;
    void *hdler_arg;
    intr_handle_t intr_handle;
} i2c_setting_t;

esp_err_t i2c_slave_begin(i2c_setting_t *i2c);
void i2c_slave_end(i2c_setting_t *i2c);
