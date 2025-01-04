
extern "C"
{
#include <i2c/hal_def.h>
// #include <i2c/hal.h>
#include <i2c/hal_drv.h>
}

namespace periph_drv
{
	// i2c slave handler callback
	using i2c_slave_read_cb_t = void (*)(i2c_setting_t *i2c, uint32_t *len);
	using i2c_slave_hdl_cb_t = void (*)(i2c_setting_t *i2c);

	template <
		i2c_slave_hdl_cb_t begin_cb,			   // handler begin
		i2c_slave_read_cb_t rx_fifo_full_cb,	   // Rx FIFO full intr
		i2c_slave_read_cb_t stop_recv_rx_cb,	   // stop-bit intr, recv rx
		i2c_slave_hdl_cb_t stop_read_cb,		   // stop-bit intr, R bit
		i2c_slave_hdl_cb_t stop_write_cb,		   // stop-bit intr, W bit
		i2c_slave_hdl_cb_t stop_finalize_cb,	   // stop-bit intr, finalizer
		i2c_slave_read_cb_t strch_recv_rx_cb,	   // stretch intr, recv rx
		i2c_slave_hdl_cb_t strch_master_read_cb,   // stretch intr, master read
		i2c_slave_hdl_cb_t strch_txfifo_empty_cb,  // stretch intr, Tx FIFO empty
		i2c_slave_hdl_cb_t strch_rxfifo_full_cb,   // stretch intr, Rx FIFO full
		i2c_slave_hdl_cb_t strch_unknown_cause_cb, // stretch intr, unknown cause
		i2c_slave_hdl_cb_t strch_finalize_cb,	   // stretch intr, finalizer
		i2c_slave_hdl_cb_t txfifo_empty_cb		   // Tx FIFO empty intr
		>
	void i2c_slave_handler_base(void *arg)
	{
		i2c_intr_info_t info;

		// I2Cドライバ取得
		i2c_setting_t *i2c = (i2c_setting_t *)arg;
		// 割り込み情報取得
		i2c_slave_get_intr_info(i2c, &info);

		// 割り込みハンドラ開始処理
		if constexpr (begin_cb != nullptr)
		{
			begin_cb(i2c);
		}

		if (info.intr_flag & I2C_RXFIFO_WM_INT_ENA)
		{
			// RX FiFo Full
			// 受信バッファフル?
			// FIFOにデータがあれば取り出す
			if constexpr (rx_fifo_full_cb != nullptr)
			{
				rx_fifo_full_cb(i2c, &info.rx_fifo_len);
			}
			// i2c_ll_slave_enable_rx_it(i2c->dev); // is this necessary?
		}

		if (info.intr_flag & I2C_TRANS_COMPLETE_INT_ENA)
		{
			// STOP bit 検出

			if (info.rx_fifo_len > 0)
			{
				// READ RX FIFO
				// FIFOにデータがあれば取り出す
				// masterからのwriteシーケンスが終了
				if constexpr (stop_recv_rx_cb != nullptr)
				{
					stop_recv_rx_cb(i2c, &info.rx_fifo_len);
				}
			}

			if (info.slave_rw)
			{
				// READ シーケンス終了
				// i2c_ll_txfifo_rst(i2c->dev);

				// READに対する応答は clock stretch 要因割り込みで処理済み
				// 受信バッファを初期化して処理完了
				if constexpr (stop_read_cb != nullptr)
				{
					stop_read_cb(i2c);
				}
			}
			else
			{
				// WRITE シーケンス終了
				if constexpr (stop_write_cb != nullptr)
				{
					stop_write_cb(i2c);
				}
			}

			// i2c_ll_txfifo_rst(i2c->dev);
			if constexpr (stop_finalize_cb != nullptr)
			{
				stop_finalize_cb(i2c);
			}
		}

		if (info.intr_flag & I2C_SLAVE_STRETCH_INT_ENA)
		{
			if (info.rx_fifo_len > 0)
			{
				if constexpr (strch_recv_rx_cb != nullptr)
				{
					strch_recv_rx_cb(i2c, &info.rx_fifo_len);
				}
			}

			// STRETCH
			// clock stretch発生後はclock stretch解除しないと通信がロックする点に注意
			// i2c_ll_stretch_clr(i2c->dev);
			// clock stretch発生
			// 発生要因？
			i2c_stretch_cause_t cause = i2c_slave_get_stretch_cause(i2c);
			if (cause == I2C_STRETCH_CAUSE_MASTER_READ)
			{
				// S ADDR W REG R ADDR R のシーケンス？
				// masterからREAD受信
				// 応答をセットしてclock stretch解除する

				if constexpr (strch_master_read_cb != nullptr)
				{
					strch_master_read_cb(i2c);
				}
			}
			else if (cause == I2C_STRETCH_CAUSE_TX_FIFO_EMPTY)
			{
				// 送信バッファempty
				if constexpr (strch_txfifo_empty_cb != nullptr)
				{
					strch_txfifo_empty_cb(i2c);
				}

				// i2c_slave_protocol1_set_ack(i2c, I2C_NACK);

				// i2c_slave_protocol1_set_tx_fifo(i2c);
				// i2c_ll_slave_disable_tx_it(i2c->dev);
			}
			else if (cause == I2C_STRETCH_CAUSE_RX_FIFO_FULL)
			{
				// 受信バッファfull
				if constexpr (strch_rxfifo_full_cb != nullptr)
				{
					strch_rxfifo_full_cb(i2c);
				}

				// i2c_slave_protocol1_set_ack(i2c, I2C_NACK);
			}
			else
			{
				// S ADDR W REG R ADDR R 以外のシーケンス？
				// マニュアルに記載がない要因が発生してる？
				// slave_byte_ack_ctl_en を有効にするとACK応答のタイミングで毎回発生する？
				// master側の送信シーケンスがおかしいとこのパスに入る

				if constexpr (strch_unknown_cause_cb != nullptr)
				{
					strch_unknown_cause_cb(i2c);
				}
				// i2c_slave_protocol1_set_ack(i2c, I2C_NACK);
				// i2c_slave_protocol1_set_tx_fifo(i2c);
			}

			if constexpr (strch_finalize_cb != nullptr)
			{
				strch_finalize_cb(i2c);
			}
		}

		if (info.intr_flag & I2C_TXFIFO_WM_INT_ENA)
		{
			// TX FiFo Empty
			if constexpr (txfifo_empty_cb != nullptr)
			{
				txfifo_empty_cb(i2c);
			}
		}
	}
}
