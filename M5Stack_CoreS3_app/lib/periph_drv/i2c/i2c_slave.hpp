#pragma once

extern "C"
{
#include "hal_drv.h"
}

namespace periph_drv
{
	template <size_t I2C_PORT>
	class i2c_slave_driver
	{
		using self_type = i2c_slave_driver<I2C_PORT>;
		i2c_setting_t i2c;

	public:
		i2c_slave_driver() : i2c()
		{

			if constexpr (I2C_PORT == 0)
			{
				// i2c_port_t ex_port = I2C_NUM_0;
				i2c.dev = &I2C0;
				i2c.module_no = PERIPH_I2C0_MODULE;
				i2c.intr_no = ETS_I2C_EXT0_INTR_SOURCE;
				i2c.signal_scl = I2CEXT0_SCL_OUT_IDX;
				i2c.signal_sda = I2CEXT0_SDA_OUT_IDX;
				i2c.intr_handle = nullptr;
			}
			else if constexpr (I2C_PORT == 1)
			{
				i2c.dev = &I2C1;
				i2c.module_no = PERIPH_I2C1_MODULE;
				i2c.intr_no = ETS_I2C_EXT1_INTR_SOURCE;
				i2c.signal_scl = I2CEXT1_SCL_OUT_IDX;
				i2c.signal_sda = I2CEXT1_SDA_OUT_IDX;
				i2c.intr_handle = nullptr;
			}
			else
			{
				static_assert("invalid");
			}
		}

		bool begin(gpio_num_t sda, gpio_num_t scl, uint16_t slave_id, uint32_t frequency, intr_handler_t handler, void *hdler_arg)
		{
			// esp32-hal-i2c-slave.cから流用
			// RTOSを使わず直接割り込みハンドラから制御する

			if (i2c.intr_handle != nullptr)
			{
				end();
			}

			// pin
			// i2c.pin_scl = (gpio_num_t)1;
			// i2c.pin_sda = (gpio_num_t)2;
			i2c.pin_scl = scl;
			i2c.pin_sda = sda;

			// baudrate
			if (!frequency)
			{
				frequency = 100000;
			}
			else if (frequency > 1000000)
			{
				frequency = 1000000;
			}
			i2c.frequency = frequency;
			i2c.slave_id = slave_id;
			i2c.handler = handler;
			i2c.hdler_arg = hdler_arg;

			esp_err_t ret = i2c_slave_begin(&i2c);

			return (ret == ESP_OK);
		}

		void end()
		{
			i2c_slave_end(&i2c);
		}

	private:
	};

	extern i2c_slave_driver<0> i2c_slave_driver_0;
}
