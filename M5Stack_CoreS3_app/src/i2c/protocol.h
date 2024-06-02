#pragma once

#include <stddef.h>
#include <stdint.h>

// protocol1定義
static const size_t protocol1_reg_size = 1; // register address size

// read応答データ用設定インターフェース
typedef struct
{
    uint8_t is_lock;
    uint8_t has_update;
    uint16_t temp_value;
    uint16_t value;
} i2c_protocol1_read_interface;

// 受信データ種別
enum protocol1_rcv_kind
{
    P1_RCV_KIND_NONE,
    P1_RCV_KIND_w_reg_r_val, // write reg STP read

    P1_RCV_KIND_MAX
};

// 受信解析状態
enum protocol1_rcv_state
{
    P1_RCV_STATE_INIT,  // 初期状態
    P1_RCV_STATE_REG,   // register address取得
    P1_RCV_STATE_WRITE, // write data取得

    MAX
};

// イベント通知インターフェース
typedef struct
{
    uint8_t has_update;
} i2c_protocol1_event_t;
extern i2c_protocol1_event_t i2c_protocol1_event;

// write受信データ用取得インターフェース
typedef struct
{
    uint8_t reg; // read対象registerがwriteされる
} i2c_protocol1_kind1_read_register;
// SOC_I2C_FIFO_LENより大きくする
static const size_t i2c_protocol1_fmt1_read_register_size = sizeof(i2c_protocol1_kind1_read_register);
#define i2c_protocol1_buf_size 128
typedef union
{
    uint8_t bytes[i2c_protocol1_buf_size];
    i2c_protocol1_kind1_read_register kind1;
} i2c_protocol1_fmt_buffer;

// 受信結果通知インターフェース
typedef struct
{
    uint8_t has_result;
    size_t state;
    size_t kind;
    size_t buff_pos;
    uint8_t ack;
    i2c_protocol1_fmt_buffer buffer;
} i2c_protocol1_result_t;
// リングバッファで保持する
#define i2c_protocol1_result_ringbuf_size 5
extern i2c_protocol1_result_t i2c_protocol1_result_ringbuf[i2c_protocol1_result_ringbuf_size];
extern size_t i2c_protocol1_result_ringbuf_pos;          // 制御上の現在参照中バッファインデックス
extern i2c_protocol1_result_t *i2c_protocol1_result_ptr; // 現在参照中バッファポインタ
extern size_t i2c_protocol1_result_ringbuf_ref_pos;      // 外部モジュールからの参照用インデックス

// debug
enum dbg_dump_seq_id
{
    DBG_DUMP_SEQ_INT_START,
    DBG_DUMP_SEQ_RXFIFO_FULL,
    DBG_DUMP_SEQ_STOP_INT_HAS_RXFIFO,
    DBG_DUMP_SEQ_STOP_INT_W,
    DBG_DUMP_SEQ_STOP_INT_R,
    DBG_DUMP_SEQ_CLK_STRTCH_INT_HAS_RXFIFO,
    DBG_DUMP_SEQ_CLK_STRTCH_INT_CAUSE_MASTER_READ,
    DBG_DUMP_SEQ_CLK_STRTCH_INT_CAUSE_TXFIFO_EMPTY,
    DBG_DUMP_SEQ_CLK_STRTCH_INT_CAUSE_RXFIFO_FULL,
    DBG_DUMP_SEQ_CLK_STRTCH_INT_CAUSE_UNDEF,
    DBG_DUMP_SEQ_TXFIFO_EMPTY,
};
typedef struct
{
    uint8_t seq_dump[255];
    size_t seq_dump_pos;
    size_t seq_dump_max;
} dbg_data_t;
extern dbg_data_t dbg_data;

void i2c_slave_isr_handler_1(void *arg);

void i2c_slave_protocol1_init(void);
void i2c_slave_protocol1_set_data1(uint16_t value);

//
void i2c_slave_protocol1_incr_buffer(void);
