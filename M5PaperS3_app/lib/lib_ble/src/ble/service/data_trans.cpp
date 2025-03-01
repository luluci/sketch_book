#include "data_trans.hpp"

namespace ble::service
{
    data_trans::data_trans() : service_base(SERVICE_UUID), dt_state_(data_trans_state::Init), data_buff(nullptr), data_buff_size(0), data_buff_cmpl(false), recv_page(0), data_recv_size(0)
    {
    }

    data_trans::~data_trans()
    {
    }

    bool data_trans::setup(BLEServer *server)
    {
        // Service作成
        auto result = service_base::setup(server);
        if (!result)
        {
            return false;
        }
        //
        tx_characteristic = service_->createCharacteristic(TX_UUID, BLECharacteristic::PROPERTY_NOTIFY);
        rx_characteristic = service_->createCharacteristic(RX_UUID, BLECharacteristic::PROPERTY_WRITE);
        tx_characteristic->addDescriptor(&tx_descriptor);
        rx_characteristic->addDescriptor(&rx_descriptor);
        tx_characteristic->setReadProperty(true);
        rx_characteristic->setWriteProperty(true);

        // tx_characteristic->setWriteNoResponseProperty(true);
        rx_characteristic->setWriteNoResponseProperty(true);

        tx_characteristic->setCallbacks(this);
        rx_characteristic->setCallbacks(this);

        return true;
    }
    void data_trans::onWrite(BLECharacteristic *pCharacteristic)
    {
        // 受信データありでタイムアウトカウンタリセット
        reset_timeout();
        // 受信データ解析
        auto buff = pCharacteristic->getData();
        auto buff_len = pCharacteristic->getLength();
        analyze_rx(buff, buff_len);
    }
    void data_trans::onRead(BLECharacteristic *pCharacteristic)
    {
        // event |= (uint16_t)data_trans_event::HasRead;
    }

    void data_trans::onNotify(BLECharacteristic *pCharacteristic)
    {
        // 自分がNotifyを通知したことのコールバック
        // event |= (uint16_t)data_trans_event::HasNotify;
    }

    void data_trans::start()
    {
        //
        service_base::start();
        //
        state_ = service_state::Active;
        event = static_cast<uint16_t>(data_trans_event::None);
    }
    void data_trans::stop()
    {
        //
        service_base::stop();
        //
        state_ = service_state::Stop;
    }
    void data_trans::notify()
    {
        tx_characteristic->setValue(resp_buff.raw, data_trans_packet_response_len);
        tx_characteristic->notify();
    }

    void data_trans::polling(uint32_t cycle_ms)
    {
        // 周期処理
        // タイムアウト監視
        timeout_timer += cycle_ms;
        if (timeout_timer > timeout_limit)
        {
            // タイムアウト
            event |= static_cast<uint16_t>(data_trans_event::DataRecvTimeout);
            init_state();
        }
    }

    void data_trans::analyze_rx(uint8_t *buff, size_t len)
    {
        //
        switch (dt_state_)
        {
        case data_trans_state::Init:
            // 初期状態受信解析
            analyze_rx_init(buff, len);
            break;

        case data_trans_state::DataRecving:
            break;

        default:
            break;
        }
    }
    void data_trans::analyze_rx_init(uint8_t *buff, size_t len)
    {
        // 転送データ内容宣言コマンドから通信開始する
        if (len != data_trans_packet_declare_len)
        {
            // エラーメッセージ
            snprintf(err_msg, err_msg_len, "Invalid packet length: %d", len);
            event |= static_cast<uint16_t>(data_trans_event::DataRecvFailed);
            notify_declare_ng(data_trans_resp_reject_reason::InvalidPacketLength);
            init_state();
            return;
        }
        // 受信データ転送
        memcpy(declare_buff.raw, buff, len);
        // ヘッダチェック
        // コマンド
        if (declare_buff.data.header.cmnd != data_trans_command::DataDeclare)
        {
            // エラーメッセージ
            snprintf(err_msg, err_msg_len, "Invalid cmnd: %d", declare_buff.data.header.cmnd);
            event |= static_cast<uint16_t>(data_trans_event::DataRecvFailed);
            notify_declare_ng(data_trans_resp_reject_reason::InvalidCommand);
            init_state();
            return;
        }
        // 転送データタイプ
        if (declare_buff.data.header.data_type >= data_trans_data_type::MAX)
        {
            // エラーメッセージ
            snprintf(err_msg, err_msg_len, "Failed invalid data_type: %d", declare_buff.data.header.data_type);
            event |= static_cast<uint16_t>(data_trans_event::DataRecvFailed);
            notify_declare_ng(data_trans_resp_reject_reason::InvalidDataType);
            init_state();
            return;
        }
        // データ宣言内容チェック
        // data_len : uint32_t表現範囲を有効にする
        // page_max : data_lenとパケットサイズから検証できるが…  とりあえずノーチェック
        // データ保持用にPSRAMからメモリ確保
        if (data_buff_size < declare_buff.data.body.data_len)
        {
            // memory確保
            data_buff_size = declare_buff.data.body.data_len;
            data_buff = psram_ptr_t(psram_malloc<uint8_t>(data_buff_size));
            if (!data_buff)
            {
                // メモリ確保失敗
                // エラーメッセージ
                snprintf(err_msg, err_msg_len, "Failed alloc PSRAM: %d", declare_buff.data.body.data_len);
                event |= static_cast<uint16_t>(data_trans_event::PSRAMAllocFailed);
                notify_declare_ng(data_trans_resp_reject_reason::FailedAllocPSRAM);
                init_state();
                return;
            }
        }

        // event: データ宣言を受理して、データ転送開始
        event |= static_cast<uint16_t>(data_trans_event::DataRecvStart);
        // データ受信状態へ遷移
        dt_state_ = data_trans_state::DataRecving;
        //
        data_recv_size = 0;
        recv_page = 0;
        data_buff_cmpl = false;
        //
        notify_declare_ok();
    }

    void data_trans::analyze_rx_data_recving(uint8_t *buff, size_t len)
    {
        // 転送データ内容受信のみ受理
        if (len != data_trans_packet_content_len)
        {
            // エラーメッセージ
            snprintf(err_msg, err_msg_len, "Invalid packet length: %d", len);
            event |= static_cast<uint16_t>(data_trans_event::DataRecvFailed);
            notify_declare_ng(data_trans_resp_reject_reason::InvalidPacketLength);
            init_state();
            return;
        }
        // 受信データ転送
        // 転送する必要あるか？
        memcpy(content_buff.raw, buff, len);
        // ヘッダチェック
        // コマンド
        if (content_buff.data.header.cmnd != data_trans_command::DataContent)
        {
            // エラーメッセージ
            snprintf(err_msg, err_msg_len, "Invalid cmnd: %d", content_buff.data.header.cmnd);
            event |= static_cast<uint16_t>(data_trans_event::DataRecvFailed);
            notify_content_ng(data_trans_resp_reject_reason::InvalidCommand);
            init_state();
            return;
        }
        // エラー発生で即通信シーケンス初期化する？  リトライ可能にする？
        // ページ番号は受信済み+1になる
        if (recv_page + 1 != content_buff.data.header.page)
        {
            // エラーメッセージ
            snprintf(err_msg, err_msg_len, "Invalid page recv: %d", content_buff.data.header.page);
            event |= static_cast<uint16_t>(data_trans_event::DataRecvFailed);
            notify_content_ng(data_trans_resp_reject_reason::InvalidPageNo);
            init_state();
            return;
        }
        // 転送データタイプが宣言と一致しているか
        if (content_buff.data.header.data_type != declare_buff.data.header.data_type)
        {
            // エラーメッセージ
            snprintf(err_msg, err_msg_len, "Invalid data_type: %d", content_buff.data.header.data_type);
            event |= static_cast<uint16_t>(data_trans_event::DataRecvFailed);
            notify_content_ng(data_trans_resp_reject_reason::InvalidDataType);
            init_state();
            return;
        }

        // データ受理
        memcpy(data_buff.get() + data_recv_size, content_buff.data.body.data, content_buff.data.header.len);
        data_recv_size += content_buff.data.header.len;
        recv_page++;
        // 全データ受信したら完了
        if (data_recv_size == declare_buff.data.body.data_len)
        {
            // 受信完了
            event |= static_cast<uint16_t>(data_trans_event::DataRecvSuccess);
            data_buff_cmpl = true;
            notify_data_complete();
            init_state();
        }
        else
        {
            // 受信継続
            event |= static_cast<uint16_t>(data_trans_event::DataRecving);
            notify_content_ok();
        }
    }

    void data_trans::notify_declare_ng(data_trans_resp_reject_reason reason)
    {
        // 転送データ宣言受信失敗を通知
        resp_buff.data.resp = data_trans_resp_type::RejectDeclare;
        resp_buff.data.detail.reject.reason = reason;
        //
        notify();
    }
    void data_trans::notify_declare_ok()
    {
        // データ宣言OKなので受信状態を通知
        resp_buff.data.resp = data_trans_resp_type::RecvStatus;
        resp_buff.data.detail.status.page = 0;
        //
        notify();
    }
    void data_trans::notify_content_ng(data_trans_resp_reject_reason reason)
    {
        // 転送データ受信失敗を通知
        resp_buff.data.resp = data_trans_resp_type::RejectContent;
        resp_buff.data.detail.reject.reason = reason;
        //
        notify();
    }
    void data_trans::notify_content_ok()
    {
        // 受信ステータスを通知
        resp_buff.data.resp = data_trans_resp_type::RecvStatus;
        resp_buff.data.detail.status.page = recv_page;
        //
        notify();
    }
    void data_trans::notify_data_complete()
    {
        // 受信完了を通知
        resp_buff.data.resp = data_trans_resp_type::RecvComplete;
        resp_buff.data.detail.status.page = recv_page;
        //
        notify();
    }

    void data_trans::init_state()
    {
        dt_state_ = data_trans_state::Init;
        reset_timeout();
    }

    void data_trans::reset_timeout()
    {
        // タイムアウトタイマリセット
        timeout_timer = 0;
    }
}
