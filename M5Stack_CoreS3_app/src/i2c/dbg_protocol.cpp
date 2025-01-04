
#include <i2c/i2c_slave_handler.hpp>

#include "dbg_protocol.h"

//
i2c_protocol1_event_t i2c_protocol1_event;
// write data (rx data)
// masterから受信したデータ
// register address
uint8_t latest_reg;							   // reg
i2c_protocol1_read_interface *latest_reg_data; // regに対応するデータ

// slave data
i2c_protocol1_read_interface data_02h;
uint8_t dummy_data[2] = {0, 0};

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
void i2c_slave_protocol1_set_tx_fifo(i2c_setting_t *i2c);
//
void i2c_slave_protocol1_check_data1(void);
void i2c_slave_protocol1_set_ack(i2c_setting_t *i2c, uint8_t ack);

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
	i2c_protocol1_result_ptr->ack = I2C_ACK;
}

void i2c_slave_protocol1_incr_buffer(void)
{
	// 受信データを確定してリングバッファを次に進める
	i2c_protocol1_result_ptr->ack = ack_state;
	i2c_protocol1_result_ringbuf[i2c_protocol1_result_ringbuf_pos].has_result = 1;
	i2c_protocol1_result_ringbuf_pos++;
	if (i2c_protocol1_result_ringbuf_size <= i2c_protocol1_result_ringbuf_pos)
	{
		i2c_protocol1_result_ringbuf_pos = 0;
	}
	if (i2c_protocol1_result_ringbuf_ref_pos == i2c_protocol1_result_ringbuf_pos)
	{
		i2c_protocol1_result_ringbuf_ref_pos++;
		if (i2c_protocol1_result_ringbuf_size <= i2c_protocol1_result_ringbuf_ref_pos)
		{
			i2c_protocol1_result_ringbuf_ref_pos = 0;
		}
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

	//
	data_02h.value = 0x1234;

	// debug
	dbg_data.seq_dump_pos = 0;
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

void i2c_slave_protocol1_analyze_rcv_reg(i2c_setting_t *i2c)
{
	switch (i2c_protocol1_result_ptr->buffer.kind1.reg)
	{
	case 0x02:
		//
		latest_reg = i2c_protocol1_result_ptr->buffer.kind1.reg;
		latest_reg_data = &data_02h;
		//
		i2c_slave_protocol1_check_data1();
		i2c_slave_protocol1_set_tx_fifo(i2c);
		// ack更新
		i2c_slave_protocol1_set_ack(i2c, I2C_ACK);
		break;

	default:
		// dummy 必要？
		// i2c_ll_write_txfifo(i2c->dev, dummy_data, 2);
		// ack更新
		i2c_slave_protocol1_set_ack(i2c, I2C_NACK);
		break;
	}
}
void i2c_slave_protocol1_set_tx_fifo(i2c_setting_t *i2c)
{
	i2c_write_tx_fifo(i2c, (uint8_t *)&(latest_reg_data->value), 2);
	latest_reg_data->value += 0x0101;
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

void i2c_slave_protocol1_set_ack(i2c_setting_t *i2c, uint8_t ack)
{
	if (ack != ack_state)
	{
		ack_state = ack;
		i2c_slave_set_ack(i2c, ack);
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

void i2c_slave_protocol1_analyze_rcv(i2c_setting_t *i2c, uint32_t *len)
{
	// 受信バッファからデータ取得
	// 必ず len > 0 でコールする前提とする
	// 暫定
	if (i2c_protocol1_result_ptr == NULL || i2c_protocol1_result_ptr->buff_pos >= i2c_protocol1_buf_size)
	{
		uint8_t buff[SOC_I2C_FIFO_LEN];
		i2c_read_rx_fifo(i2c, buff, (uint8_t)*len);
		return;
	}

	uint8_t *buff = &(i2c_protocol1_result_ptr->buffer.bytes[i2c_protocol1_result_ptr->buff_pos]);
	i2c_read_rx_fifo(i2c, buff, (uint8_t)*len);
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

void i2c_slave_protocol1_begin(i2c_setting_t *i2c)
{
	// i2c-slave割り込みハンドラ開始
	dbg_i2c_slave_protocol1_dump_seq(DBG_DUMP_SEQ_INT_START);
}
void i2c_slave_protocol1_rx_fifo_full(i2c_setting_t *i2c, uint32_t *len)
{
	// RxFIFOバッファフル割り込み
	dbg_i2c_slave_protocol1_dump_seq(DBG_DUMP_SEQ_RXFIFO_FULL);
	// RxFIFOから受信データを取り出す
	i2c_slave_protocol1_analyze_rcv(i2c, len);
}
void i2c_slave_protocol1_stop_recv_rx(i2c_setting_t *i2c, uint32_t *len)
{
	// stop-bit割り込み: 受信あり
	// stop-bit+受信あり、はつまりmasterからWRITEが完了したタイミング
	// プロトコル次第でWRITEで終了するかREADが続くかが変わるはず
	dbg_i2c_slave_protocol1_dump_seq(DBG_DUMP_SEQ_STOP_INT_HAS_RXFIFO);
	// RxFIFOから受信データを取り出す
	i2c_slave_protocol1_analyze_rcv(i2c, len);
}
void i2c_slave_protocol1_stop_read(i2c_setting_t *i2c)
{
	// stop-bit割り込み: Rフラグ
	// READ シーケンス終了
	dbg_i2c_slave_protocol1_dump_seq(DBG_DUMP_SEQ_STOP_INT_R);
	// i2c_ll_txfifo_rst(i2c->dev);

	// READに対する応答は clock stretch 要因割り込みで処理済み
	// 受信バッファを初期化して処理完了
	// i2c_slave_protocol1_init_buffer(i2c_protocol1_result_ringbuf_pos);
	// 受信バッファ確定する
	i2c_slave_protocol1_incr_buffer();

	//
	i2c_protocol1_event.has_update = 1;
}
void i2c_slave_protocol1_stop_write(i2c_setting_t *i2c)
{
	// stop-bit割り込み: Wフラグ
	// WRITE シーケンス終了
	//
	// 受信バッファ確定する
	i2c_slave_protocol1_incr_buffer();

	dbg_i2c_slave_protocol1_dump_seq(DBG_DUMP_SEQ_STOP_INT_W);
	//
	i2c_protocol1_event.has_update = 1;
}
void i2c_slave_protocol1_stop_finalize(i2c_setting_t *i2c)
{
	// stop-bit割り込み: 終了処理

	// i2c_ll_txfifo_rst(i2c->dev);
	// STOPbitでACK設定をリセット
	i2c_slave_protocol1_set_ack(i2c, I2C_ACK);
}
void i2c_slave_protocol1_stretch_recv_rx(i2c_setting_t *i2c, uint32_t *len)
{
	// clock-stretch割り込み: 受信あり
	dbg_i2c_slave_protocol1_dump_seq(DBG_DUMP_SEQ_CLK_STRTCH_INT_HAS_RXFIFO);

	i2c_slave_protocol1_analyze_rcv(i2c, len);
}
void i2c_slave_protocol1_stretch_master_read(i2c_setting_t *i2c)
{
	// clock-stretch割り込み: READ受信
	dbg_i2c_slave_protocol1_dump_seq(DBG_DUMP_SEQ_CLK_STRTCH_INT_CAUSE_MASTER_READ);

	// S ADDR W REG R ADDR R のシーケンス？
	// masterからREAD受信
	// 応答をセットしてclock stretch解除する

	// i2c_slave_protocol1_set_tx_fifo(i2c);
}
void i2c_slave_protocol1_stretch_tx_fifo_empty(i2c_setting_t *i2c)
{
	// clock-stretch割り込み: TxFIFO empty
	// 送信バッファempty
	dbg_i2c_slave_protocol1_dump_seq(DBG_DUMP_SEQ_CLK_STRTCH_INT_CAUSE_TXFIFO_EMPTY);

	// i2c_slave_protocol1_set_ack(i2c, I2C_NACK);

	// i2c_slave_protocol1_set_tx_fifo(i2c);
	// i2c_ll_slave_disable_tx_it(i2c->dev);
}
void i2c_slave_protocol1_stretch_rx_fifo_full(i2c_setting_t *i2c)
{
	// clock-stretch割り込み: RxFIFO full
	// 受信バッファfull
	dbg_i2c_slave_protocol1_dump_seq(DBG_DUMP_SEQ_CLK_STRTCH_INT_CAUSE_RXFIFO_FULL);

	// i2c_slave_protocol1_set_ack(i2c, I2C_NACK);
}
void i2c_slave_protocol1_stretch_unknown_cause(i2c_setting_t *i2c)
{
	// clock-stretch割り込み: マイコン未定義要因
	dbg_i2c_slave_protocol1_dump_seq(DBG_DUMP_SEQ_CLK_STRTCH_INT_CAUSE_UNDEF);

	// S ADDR W REG R ADDR R 以外のシーケンス？
	// マニュアルに記載がない要因が発生してる？
	// slave_byte_ack_ctl_en を有効にするとACK応答のタイミングで毎回発生する？

	// i2c_slave_protocol1_set_ack(i2c, I2C_NACK);
	// i2c_slave_protocol1_set_tx_fifo(i2c);
}
void i2c_slave_protocol1_stretch_finalizer(i2c_setting_t *i2c)
{
	// clock-stretch割り込み: 終了処理
	// clock stretch解除
	i2c_slave_clear_clock_stretch(i2c);
}
void i2c_slave_protocol1_tx_fifo_empty(i2c_setting_t *i2c)
{
	// TX FiFo Empty
	dbg_i2c_slave_protocol1_dump_seq(DBG_DUMP_SEQ_TXFIFO_EMPTY);
}

intr_handler_t dbg_protocol_handler = periph_drv::i2c_slave_handler_base<
	i2c_slave_protocol1_begin,
	i2c_slave_protocol1_rx_fifo_full,
	i2c_slave_protocol1_stop_recv_rx,
	i2c_slave_protocol1_stop_read,
	i2c_slave_protocol1_stop_write,
	i2c_slave_protocol1_stop_finalize,
	i2c_slave_protocol1_stretch_recv_rx,
	i2c_slave_protocol1_stretch_master_read,
	i2c_slave_protocol1_stretch_tx_fifo_empty,
	i2c_slave_protocol1_stretch_rx_fifo_full,
	i2c_slave_protocol1_stretch_unknown_cause,
	i2c_slave_protocol1_stretch_finalizer,
	i2c_slave_protocol1_tx_fifo_empty>;
