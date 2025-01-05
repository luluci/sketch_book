#pragma once

#include <driver/periph_ctrl.h>
#include <driver/i2c.h>
#include <soc/soc.h>
#include <soc/i2c_reg.h>
#include <soc/i2c_struct.h>

#include "hal_def.h"

// I2Cドライバ設定情報
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

// I2C割り込み情報
typedef struct i2c_intr_info
{
    uint32_t intr_flag;   // 割り込み要因フラグ
    uint32_t rx_fifo_len; // RxFIFO 受信データ数
    bool slave_rw;        // R/Wフラグ
} i2c_intr_info_t;

esp_err_t i2c_slave_begin(i2c_setting_t *i2c);
void i2c_slave_end(i2c_setting_t *i2c);

// ESP32のペリフェラル操作関数はC言語依存で記述されているため、C++と互換性が無い
// C++コードから呼び出すとコンパイルエラーになるため、Cコードのラッパーを経由する。
// そのため、関数コール分のオーバーヘッドが生じる。

void i2c_write_tx_fifo(i2c_setting_t *i2c, uint8_t *buff, uint8_t len);
void i2c_read_rx_fifo(i2c_setting_t *i2c, uint8_t *buff, uint8_t len);
// I2C割り込み情報取得
void i2c_slave_get_intr_info(i2c_setting_t *i2c, i2c_intr_info_t *info);
void i2c_slave_set_ack(i2c_setting_t *i2c, uint8_t ack);
void i2c_slave_clear_clock_stretch(i2c_setting_t *i2c);
i2c_stretch_cause_t i2c_slave_get_stretch_cause(i2c_setting_t *i2c);
