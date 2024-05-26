#include "protocol.h"

#include <i2c/hal.h>
#include <i2c/hal_drv.h>

//
i2c_protocol1_event_t i2c_protocol1_event;
// write data (rx data)
// masterから受信したデータ
// register address
uint8_t latest_reg;							   // reg
i2c_protocol1_read_interface *latest_reg_data; // regに対応するデータ

// slave data
i2c_protocol1_read_interface data_02h;

i2c_protocol1_result_t i2c_protocol1_result_ringbuf[i2c_protocol1_result_ringbuf_size];
size_t i2c_protocol1_result_ringbuf_pos;
i2c_protocol1_result_t *i2c_protocol1_result_ptr = NULL;
size_t i2c_protocol1_result_ringbuf_ref_pos;

uint8_t ack_state = 1;

// debug
dbg_data_t dbg_data;

enum protocol_state
{
	s1_idle,
};

// 受信データ解析
void i2c_slave_protocol1_analyze_rcv_reg(i2c_setting_t *i2c);

//
void i2c_slave_protocol1_check_data1(void);

// プロトコル管理
void i2c_slave_protocol1_analyze_rcv(i2c_setting_t *i2c, uint32_t *len);

// debug
void dbg_i2c_slave_protocol1_dump_seq(uint8_t seq);

void i2c_slave_protocol1_init_buffer(size_t pos)
{
	//
	i2c_protocol1_result_ringbuf_pos = pos;
	if (i2c_protocol1_result_ringbuf_size <= i2c_protocol1_result_ringbuf_pos)
	{
		i2c_protocol1_result_ringbuf_pos = 0;
	}
	//
	i2c_protocol1_result_ptr = &(i2c_protocol1_result_ringbuf[i2c_protocol1_result_ringbuf_pos]);
	i2c_protocol1_result_ptr->has_result = 0;
	i2c_protocol1_result_ptr->buff_pos = 0;
	i2c_protocol1_result_ptr->state = P1_RCV_STATE_INIT;
	i2c_protocol1_result_ptr->kind = P1_RCV_KIND_NONE;
	i2c_protocol1_result_ptr->ack = 1;
}

void i2c_slave_protocol1_incr_buffer(void)
{
	// 受信データを確定してリングバッファを次に進める
	i2c_protocol1_result_ringbuf_pos++;
	if (i2c_protocol1_result_ringbuf_size <= i2c_protocol1_result_ringbuf_pos)
	{
		i2c_protocol1_result_ringbuf_pos = 0;
	}
	// 更新したポインタで受信バッファ初期化
	i2c_slave_protocol1_init_buffer(i2c_protocol1_result_ringbuf_pos);
}

void i2c_slave_protocol1_init(void)
{
	//
	i2c_slave_protocol1_init_buffer(0);
	//
	i2c_protocol1_result_ringbuf_ref_pos = 0;

	// debug
	dbg_data.seq_dump_pos = 0;
}

void i2c_slave_isr_handler_1(void *arg)
{
	bool pxHigherPriorityTaskWoken = false;
	i2c_setting_t *i2c = (i2c_setting_t *)arg;

	uint32_t activeInt = i2c_ll_get_intsts_mask(i2c->dev);
	i2c_ll_clr_intsts_mask(i2c->dev, activeInt);
	uint32_t rx_fifo_len = i2c_ll_get_rxfifo_cnt(i2c->dev);
	bool slave_rw = i2c_ll_slave_rw(i2c->dev);

	if (activeInt & I2C_RXFIFO_WM_INT_ENA)
	{
		// RX FiFo Full
		// 受信バッファフル?
		// 後で消す pxHigherPriorityTaskWoken |= i2c_slave_handle_rx_fifo_full(i2c, rx_fifo_len);
		i2c_slave_protocol1_analyze_rcv(i2c, &rx_fifo_len);
		i2c_ll_slave_enable_rx_it(i2c->dev); // is this necessary?

		dbg_i2c_slave_protocol1_dump_seq(DBG_DUMP_SEQ_RXFIFO_FULL);
	}

	if (activeInt & I2C_TRANS_COMPLETE_INT_ENA)
	{
		// STOP bit 検出

		if (rx_fifo_len > 0)
		{
			// READ RX FIFO
			// FIFOにデータがあれば取り出す
			// masterからのwriteシーケンスが終了
			i2c_slave_protocol1_analyze_rcv(i2c, &rx_fifo_len);

			// pxHigherPriorityTaskWoken |= i2c_slave_handle_rx_fifo_full(i2c, rx_fifo_len);

			dbg_i2c_slave_protocol1_dump_seq(DBG_DUMP_SEQ_STOP_INT_HAS_RXFIFO);
		}

		if (slave_rw)
		{
			// READ シーケンス終了

			// READに対する応答は clock stretch 要因割り込みで処理済み
			// 受信バッファを初期化して処理完了
			// i2c_slave_protocol1_init_buffer(i2c_protocol1_result_ringbuf_pos);
			// 受信バッファ確定する
			i2c_slave_protocol1_incr_buffer();

			dbg_i2c_slave_protocol1_dump_seq(DBG_DUMP_SEQ_STOP_INT_R);
			//
			i2c_protocol1_event.has_update = 1;
		}
		else
		{
			// WRITE シーケンス終了
			//
			// 受信バッファ確定する
			i2c_slave_protocol1_incr_buffer();

			dbg_i2c_slave_protocol1_dump_seq(DBG_DUMP_SEQ_STOP_INT_W);
			//
			i2c_protocol1_event.has_update = 1;
		}

		// 		if (i2c->rx_data_count)
		// 		{
		// 			// WRITE or RepeatedStart
		// 			// 何かしら受信していたら

		// 			// SEND RX Event
		// 			i2c_slave_queue_event_t event;
		// 			event.event = I2C_SLAVE_EVT_RX;
		// 			event.stop = !slave_rw;
		// 			event.param = i2c->rx_data_count;
		// 			pxHigherPriorityTaskWoken |= i2c_slave_send_event(i2c, &event);
		// 			// Zero RX count
		// 			i2c->rx_data_count = 0;
		// 		}
		// 		if (slave_rw)
		// 		{ // READ
		// #if CONFIG_IDF_TARGET_ESP32
		// 			if (i2c->dev->status_reg.scl_main_state_last == 6)
		// 			{
		// 				// SEND TX Event
		// 				i2c_slave_queue_event_t event;
		// 				event.event = I2C_SLAVE_EVT_TX;
		// 				pxHigherPriorityTaskWoken |= i2c_slave_send_event(i2c, &event);
		// 			}
		// #else
		// 		  // reset TX data
		// 			i2c_ll_txfifo_rst(i2c->dev);
		// 			uint8_t d;
		// 			while (xQueueReceiveFromISR(i2c->tx_queue, &d, (BaseType_t *const)&pxHigherPriorityTaskWoken) == pdTRUE)
		// 				; // flush partial write
		// #endif
		// 		}
	}

#ifndef CONFIG_IDF_TARGET_ESP32
	if (activeInt & I2C_SLAVE_STRETCH_INT_ENA)
	{
		if (rx_fifo_len > 0)
		{
			// pxHigherPriorityTaskWoken |= i2c_slave_handle_rx_fifo_full(i2c, rx_fifo_len);
			i2c_slave_protocol1_analyze_rcv(i2c, &rx_fifo_len);

			dbg_i2c_slave_protocol1_dump_seq(DBG_DUMP_SEQ_CLK_STRTCH_INT_HAS_RXFIFO);
		}

		// STRETCH
		// clock stretch発生
		// 発生要因？
		i2c_stretch_cause_t cause = i2c_ll_stretch_cause(i2c->dev);
		if (cause == I2C_STRETCH_CAUSE_MASTER_READ)
		{
			// S ADDR W REG R ADDR R のシーケンス？
			// masterからREAD受信
			// 応答をセットしてclock stretch解除する

			// on C3 RX data dissapears with repeated start, so we need to get it here
			// if (rx_fifo_len)
			// {
			// 	pxHigherPriorityTaskWoken |= i2c_slave_handle_rx_fifo_full(i2c, rx_fifo_len);
			// }
			// SEND TX Event
			// i2c_slave_queue_event_t event;
			// event.event = I2C_SLAVE_EVT_TX;
			// pxHigherPriorityTaskWoken |= i2c_slave_send_event(i2c, &event);
			// will clear after execution
			dbg_i2c_slave_protocol1_dump_seq(DBG_DUMP_SEQ_CLK_STRTCH_INT_CAUSE_MASTER_READ);
		}
		else if (cause == I2C_STRETCH_CAUSE_TX_FIFO_EMPTY)
		{
			// 送信バッファempty

			// pxHigherPriorityTaskWoken |= i2c_slave_handle_tx_fifo_empty(i2c);
			// i2c_ll_stretch_clr(i2c->dev);
			dbg_i2c_slave_protocol1_dump_seq(DBG_DUMP_SEQ_CLK_STRTCH_INT_CAUSE_TXFIFO_EMPTY);
		}
		else if (cause == I2C_STRETCH_CAUSE_RX_FIFO_FULL)
		{
			// 受信バッファfull

			// pxHigherPriorityTaskWoken |= i2c_slave_handle_rx_fifo_full(i2c, rx_fifo_len);
			// i2c_ll_stretch_clr(i2c->dev);
			dbg_i2c_slave_protocol1_dump_seq(DBG_DUMP_SEQ_CLK_STRTCH_INT_CAUSE_RXFIFO_FULL);
		}
		else
		{
			// S ADDR W REG R ADDR R 以外のシーケンス？
			// マニュアルに記載がない要因が発生してる？
			// slave_byte_ack_ctl_en を有効にするとACK応答のタイミングで毎回発生する？

			dbg_i2c_slave_protocol1_dump_seq(DBG_DUMP_SEQ_CLK_STRTCH_INT_CAUSE_UNDEF);
		}

		// clock stretch解除
		i2c_ll_set_ack(i2c->dev, ack_state);
		i2c_ll_stretch_clr(i2c->dev);
	}
#endif

	if (activeInt & I2C_TXFIFO_WM_INT_ENA)
	{ // TX FiFo Empty
	  // pxHigherPriorityTaskWoken |= i2c_slave_handle_tx_fifo_empty(i2c);
		dbg_i2c_slave_protocol1_dump_seq(DBG_DUMP_SEQ_TXFIFO_EMPTY);
	}

	// if (pxHigherPriorityTaskWoken)
	// {
	// 	portYIELD_FROM_ISR();
	// }
}

void i2c_slave_protocol1_set_data1(uint16_t value)
{
	// 本関数はタスクからコールするインターフェース
	// データ書き込み時にlockする
	// lock時はI2C割り込み側が何もしないこととする
	data_02h.is_lock = 1;

	// 非lock時にI2C割り込みからインターフェースをチェックしたとき
	// temp_valueからvalueにデータを移す
	data_02h.temp_value = value;
	data_02h.has_update = 1;

	data_02h.is_lock = 0;
}

void i2c_slave_protocol1_check_data1(void)
{
	// I2C割り込みからは非lock時のみデータを更新する
	// lock時は前回データをそのまま使用する
	if (data_02h.is_lock == 0)
	{
		if (data_02h.has_update == 1)
		{
			data_02h.value = data_02h.temp_value;
			data_02h.has_update = 0;
		}
	}
}

void i2c_slave_protocol1_analyze_rcv(i2c_setting_t *i2c, uint32_t *len)
{
	// 受信バッファからデータ取得
	// 必ず len > 0 でコールする前提とする
	// 暫定
	if (i2c_protocol1_result_ptr == NULL || i2c_protocol1_result_ptr->buff_pos >= i2c_protocol1_buf_size)
	{
		uint8_t buff[SOC_I2C_FIFO_LEN];
		i2c_ll_read_rxfifo(i2c->dev, buff, (uint8_t)*len);
		return;
	}

	uint8_t *buff = &(i2c_protocol1_result_ptr->buffer.bytes[i2c_protocol1_result_ptr->buff_pos]);
	i2c_ll_read_rxfifo(i2c->dev, buff, (uint8_t)*len);
	i2c_protocol1_result_ptr->buff_pos += *len;
	*len = 0;

	// 受信解析:状態遷移
	switch (i2c_protocol1_result_ptr->state)
	{
	case P1_RCV_STATE_INIT:
		if (protocol1_reg_size <= i2c_protocol1_result_ptr->buff_pos)
		{
			// reg解析
			i2c_protocol1_result_ptr->state = P1_RCV_STATE_REG;
			i2c_slave_protocol1_analyze_rcv_reg(i2c);
		}
		break;

	case P1_RCV_STATE_REG:
		i2c_protocol1_result_ptr->state = P1_RCV_STATE_WRITE;
		break;

	default:
		break;
	}
}

void i2c_slave_protocol1_analyze_rcv_reg(i2c_setting_t *i2c)
{
	switch (i2c_protocol1_result_ptr->buffer.kind1.reg)
	{
	case 0x02:
		i2c_slave_protocol1_check_data1();
		i2c_ll_write_txfifo(i2c->dev, (uint8_t *)&(data_02h.value), 2);

		//
		latest_reg = i2c_protocol1_result_ptr->buffer.kind1.reg;
		latest_reg_data = &data_02h;
		ack_state = 1;
		i2c_protocol1_result_ptr->ack = ack_state;
		break;

	default:
		ack_state = 0;
		i2c_protocol1_result_ptr->ack = ack_state;
		break;
	}
}

void dbg_i2c_slave_protocol1_dump_seq(uint8_t seq)
{
	if (dbg_data.seq_dump_pos >= 255)
	{
		return;
	}

	dbg_data.seq_dump[dbg_data.seq_dump_pos] = seq;
	dbg_data.seq_dump_pos++;
}