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

#include "hal_drv.h"

#include "hal.h"

void i2c_set_frequency(i2c_setting_t *i2c);
bool i2c_check_line_state(i2c_setting_t *i2c);
bool i2c_attach_gpio(i2c_setting_t *i2c);
void i2c_set_gpio_mode(int8_t pin, gpio_mode_t mode);
void i2c_delay_us(uint64_t us);

bool i2c_slave_detach_gpio(i2c_setting_t *i2c);

esp_err_t i2c_slave_begin(i2c_setting_t *i2c)
{

    //
    periph_module_enable(i2c->module_no);

    i2c_ll_slave_init(i2c->dev);
    i2c_ll_set_fifo_mode(i2c->dev, true);
    i2c_ll_set_slave_addr(i2c->dev, i2c->slave_id, false);
    // i2c_ll_set_tout(i2c->dev, I2C_LL_MAX_TIMEOUT);
    i2c_ll_set_tout(i2c->dev, 0x1F);
    //
    i2c_set_frequency(i2c);
    //

    if (!i2c_check_line_state(i2c))
    {
        // error
        // log_e("bad pin state");
        i2c_slave_end(i2c);
        return ESP_FAIL;
    }

    i2c_attach_gpio(i2c);

    if (i2c_ll_is_bus_busy(i2c->dev))
    {
        // error
        // log_w("Bus busy, reinit");
        i2c_slave_end(i2c);
        return ESP_FAIL;
    }

    i2c_ll_disable_intr_mask(i2c->dev, I2C_LL_INTR_MASK);
    i2c_ll_clr_intsts_mask(i2c->dev, I2C_LL_INTR_MASK);
    i2c_ll_set_fifo_mode(i2c->dev, true);

    esp_err_t ret = ESP_OK;
    uint32_t flags = ESP_INTR_FLAG_LOWMED | ESP_INTR_FLAG_SHARED;
    ret = esp_intr_alloc(i2c->intr_no, flags, i2c->handler, i2c, &i2c->intr_handle);
    if (ret != ESP_OK)
    {
        // log_e("install interrupt handler Failed=%d", ret);
        i2c_slave_end(i2c);
        return ret;
    }

    i2c_ll_txfifo_rst(i2c->dev);
    i2c_ll_rxfifo_rst(i2c->dev);
    i2c_ll_slave_enable_rx_it(i2c->dev);
    i2c_ll_set_stretch(i2c->dev, 0x3FF);
    i2c_ll_update(i2c->dev);

    return ret;
}

void i2c_set_frequency(i2c_setting_t *i2c)
{
    if (i2c->frequency > 1100000UL)
    {
        i2c->frequency = 1100000UL;
    }

    // Adjust Fifo thresholds based on frequency
    uint32_t a = (i2c->frequency / 50000L) + 2;
    // log_d("Fifo thresholds: rx_fifo_full = %d, tx_fifo_empty = %d", SOC_I2C_FIFO_LEN - a, a);

    i2c_clk_cal_t clk_cal;
#if SOC_I2C_SUPPORT_APB
    i2c_ll_cal_bus_clk(APB_CLK_FREQ, clk_speed, &clk_cal);
    i2c_ll_set_source_clk(dev, I2C_SCLK_APB); /*!< I2C source clock from APB, 80M*/
#elif SOC_I2C_SUPPORT_XTAL
    i2c_ll_cal_bus_clk(XTAL_CLK_FREQ, i2c->frequency, &clk_cal);
    i2c_ll_set_source_clk(i2c->dev, I2C_SCLK_XTAL); /*!< I2C source clock from XTAL, 40M */
#endif
    i2c_ll_set_txfifo_empty_thr(i2c->dev, a);
    i2c_ll_set_rxfifo_full_thr(i2c->dev, SOC_I2C_FIFO_LEN - a);
    i2c_ll_set_bus_timing(i2c->dev, &clk_cal);
    i2c_ll_set_filter(i2c->dev, 3);
}

bool i2c_check_line_state(i2c_setting_t *i2c)
{
    if (i2c->pin_sda < 0 || i2c->pin_scl < 0)
    {
        return false; // return false since there is nothing to do
    }
    // if the bus is not 'clear' try the cycling SCL until SDA goes High or 9 cycles
    gpio_set_level(i2c->pin_sda, 1);
    gpio_set_level(i2c->pin_scl, 1);
    i2c_set_gpio_mode(i2c->pin_sda, GPIO_MODE_INPUT | GPIO_MODE_DEF_OD);
    i2c_set_gpio_mode(i2c->pin_scl, GPIO_MODE_INPUT | GPIO_MODE_DEF_OD);
    gpio_set_level(i2c->pin_scl, 1);

    if (!gpio_get_level(i2c->pin_sda) || !gpio_get_level(i2c->pin_scl))
    { // bus in busy state
        // log_w("invalid state sda(%d)=%d, scl(%d)=%d", sda, gpio_get_level(sda), scl, gpio_get_level(scl));
        for (uint8_t a = 0; a < 9; a++)
        {
            i2c_delay_us(5);
            if (gpio_get_level(i2c->pin_sda) && gpio_get_level(i2c->pin_scl))
            { // bus recovered
                // log_w("Recovered after %d Cycles", a);
                gpio_set_level(i2c->pin_sda, 0); // start
                i2c_delay_us(5);
                for (uint8_t a = 0; a < 9; a++)
                {
                    gpio_set_level(i2c->pin_scl, 1);
                    i2c_delay_us(5);
                    gpio_set_level(i2c->pin_scl, 0);
                    i2c_delay_us(5);
                }
                gpio_set_level(i2c->pin_scl, 1);
                i2c_delay_us(5);
                gpio_set_level(i2c->pin_sda, 1); // stop
                break;
            }
            gpio_set_level(i2c->pin_scl, 0);
            i2c_delay_us(5);
            gpio_set_level(i2c->pin_scl, 1);
        }
    }

    if (!gpio_get_level(i2c->pin_sda) || !gpio_get_level(i2c->pin_scl))
    { // bus in busy state
        // log_e("Bus Invalid State, Can't init sda=%d, scl=%d", gpio_get_level(sda), gpio_get_level(scl));
        return false; // bus is busy
    }
    return true;
}

bool i2c_attach_gpio(i2c_setting_t *i2c)
{
    gpio_set_level(i2c->pin_scl, 1);
    i2c_set_gpio_mode(i2c->pin_scl, GPIO_MODE_INPUT_OUTPUT_OD);
    gpio_matrix_out(i2c->pin_scl, i2c->signal_scl, false, false);
    gpio_matrix_in(i2c->pin_scl, i2c->signal_scl, false);

    gpio_set_level(i2c->pin_sda, 1);
    i2c_set_gpio_mode(i2c->pin_sda, GPIO_MODE_INPUT_OUTPUT_OD);
    gpio_matrix_out(i2c->pin_sda, i2c->signal_sda, false, false);
    gpio_matrix_in(i2c->pin_sda, i2c->signal_sda, false);

    return true;
}

void i2c_set_gpio_mode(int8_t pin, gpio_mode_t mode)
{
    gpio_config_t conf = {
        .pin_bit_mask = 1LL << pin,
        .mode = mode,
        .pull_up_en = GPIO_PULLUP_ENABLE,
        .pull_down_en = GPIO_PULLDOWN_DISABLE,
        .intr_type = GPIO_INTR_DISABLE};
    gpio_config(&conf);
}

void i2c_delay_us(uint64_t us)
{
    uint64_t m = esp_timer_get_time();
    if (us)
    {
        uint64_t e = (m + us);
        if (m > e)
        { // overflow
            while ((uint64_t)esp_timer_get_time() > e)
                ;
        }
        while ((uint64_t)esp_timer_get_time() < e)
            ;
    }
}

void i2c_slave_end(i2c_setting_t *i2c)
{
    i2c_slave_detach_gpio(i2c);
    i2c_ll_set_slave_addr(i2c->dev, 0, false);
    i2c_ll_disable_intr_mask(i2c->dev, I2C_LL_INTR_MASK);
    i2c_ll_clr_intsts_mask(i2c->dev, I2C_LL_INTR_MASK);

    if (i2c->intr_handle != NULL)
    {
        esp_intr_free(i2c->intr_handle);
        i2c->intr_handle = NULL;
    }
}

bool i2c_slave_detach_gpio(i2c_setting_t *i2c)
{
    if (i2c == NULL)
    {
        // log_e("no control Block");
        return false;
    }
    if (i2c->pin_scl >= 0)
    {
        gpio_matrix_out(i2c->pin_scl, 0x100, false, false);
        gpio_matrix_in(0x30, i2c->signal_scl, false);
        i2c_set_gpio_mode(i2c->pin_scl, GPIO_MODE_INPUT);
        i2c->pin_scl = -1; // un attached
    }
    if (i2c->pin_sda >= 0)
    {
        gpio_matrix_out(i2c->pin_sda, 0x100, false, false);
        gpio_matrix_in(0x30, i2c->signal_sda, false);
        i2c_set_gpio_mode(i2c->pin_sda, GPIO_MODE_INPUT);
        i2c->pin_sda = -1; // un attached
    }
    return true;
}
