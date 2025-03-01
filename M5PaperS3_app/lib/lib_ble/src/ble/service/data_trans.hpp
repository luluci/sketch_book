#pragma once

#include "../defines.hpp"
#include "service_base.hpp"

#include <BLEServer.h>
#include <BLEUtils.h>
#include <BLE2902.h>

#include <memory>
#include <array>
#include <string>

namespace ble::service
{
    enum class data_trans_event : uint16_t
    {
        None = 0,
        DataRecvStart = (1 << 0),
        DataRecving = (1 << 1),
        DataRecvSuccess = (1 << 2),
        DataRecvFailed = (1 << 3),
        DataRecvTimeout = (1 << 4),
        PSRAMAllocFailed = (1 << 5),

        MAX
    };

    enum class data_trans_state : uint8_t
    {
        Init,        // 初期状態
        DataRecving, // 転送データ受信中状態
        DataRecved,  // データ受信完了状態

        MAX
    };

    // 通信プロトコル
    enum data_trans_command : uint8_t
    {
        DataDeclare, // 転送データ宣言
        DataContent, // 転送データ内容
    };
    enum data_trans_data_type : uint8_t
    {
        file_PNG,

        MAX
    };
    enum data_trans_resp_type : uint8_t
    {
        // 正常系
        RecvStatus,   // 受信ステータス
        RecvComplete, // 受信完了

        // 異常系
        RejectDeclare, // 転送データ宣言拒否
        RejectContent, // 転送データ内容拒否
    };
    enum data_trans_resp_reject_reason : uint8_t
    {
        InvalidPacketLength,
        InvalidCommand,
        InvalidDataType,
        FailedAllocPSRAM,
        InvalidPageNo,
    };
    // 受信パケット定義
    // BLEパケットMTU=257バイト
    // 128バイト/1パケットでプロトコルを構築する
    constexpr size_t data_trans_packet_len = 128;
    struct data_trans_packet_header
    {
        data_trans_command cmnd;        // コマンド
        uint8_t len;                    // ボディサイズ
        data_trans_data_type data_type; // 転送データの内容
        uint8_t page;                   // 転送データページ番号(0 ~ page_max-1)
        uint32_t reserve;               // reserve
    };
    constexpr size_t data_trans_packet_header_len = sizeof(data_trans_packet_header);
    constexpr size_t data_trans_packet_body_len = data_trans_packet_len - data_trans_packet_header_len;
    struct data_trans_packet_declare
    {
        data_trans_packet_header header;
        struct
        {
            uint32_t data_len; // 転送データ全体サイズ
            uint32_t page_max; // 転送データ全体分割数
        } body;
    };
    struct data_trans_packet_content
    {
        data_trans_packet_header header;
        struct
        {
            uint8_t data[data_trans_packet_body_len];
        } body;
    };
    constexpr size_t data_trans_packet_declare_len = sizeof(data_trans_packet_declare);
    constexpr size_t data_trans_packet_content_len = sizeof(data_trans_packet_content);
    union data_trans_packet_declare_buffer
    {
        uint8_t raw[data_trans_packet_declare_len];
        data_trans_packet_declare data;
    };
    static_assert(data_trans_packet_declare_len == sizeof(data_trans_packet_declare_buffer));
    union data_trans_packet_content_buffer
    {
        uint8_t raw[data_trans_packet_content_len];
        data_trans_packet_content data;
    };
    static_assert(data_trans_packet_content_len == sizeof(data_trans_packet_content_buffer));

    // 送信パケット定義
    struct data_trans_packet_response
    {
        data_trans_resp_type resp; // コマンド
        uint8_t reserve1;
        uint8_t reserve2;
        uint8_t reserve3;
        union
        {
            // ステータス情報詳細
            struct
            {
                uint32_t page; // 受信済みページ番号
            } status;
            // 転送データ宣言拒否詳細
            struct
            {
                data_trans_resp_reject_reason reason;
            } reject;
        } detail;
    };
    constexpr size_t data_trans_packet_response_len = sizeof(data_trans_packet_response);
    union data_trans_packet_response_buffer
    {
        uint8_t raw[data_trans_packet_response_len];
        data_trans_packet_response data;
    };
    static_assert(data_trans_packet_response_len == sizeof(data_trans_packet_response_buffer));

    class data_trans : public service_base,
                       public BLECharacteristicCallbacks
    {
    public:
        uint16_t event;
        uint16_t get_event()
        {
            auto result = event;
            event = static_cast<uint16_t>(data_trans_event::None);
            return result;
        }

    private:
        static constexpr char const *const SERVICE_UUID = "37ea2bc7-86d9-4c8d-9e38-3d5ff19995f4";
        static constexpr char const *const RX_UUID = "537b7e5c-599d-4465-a708-a314ee9b3b37";
        static constexpr char const *const TX_UUID = "6fcfe024-2586-4e1f-931a-5e9e3a8a12c7";
        // Serial Characteristics
        BLECharacteristic *tx_characteristic;
        BLECharacteristic *rx_characteristic;
        BLE2902 tx_descriptor;
        BLE2902 rx_descriptor;

        // タイムアウト監視
        uint32_t timeout_timer;                              // タイムアウトカウンタ(ms)
        static constexpr uint32_t timeout_limit = 1000 * 10; // タイムアウト時間(ms)

        // PSRAM
        template <typename T>
        T *psram_malloc(size_t size)
        {
            return reinterpret_cast<T *>(heap_caps_malloc(size, MALLOC_CAP_SPIRAM | MALLOC_CAP_8BIT));
        }
        template <typename T>
        struct PSRAMDeleter
        {
            /*!
             * @brief デリート処理を行うオペレータ
             * @param [in,out] p  アラインメントされたメモリ領域へのポインタ
             */
            void
            operator()(T *p) const noexcept
            {
                heap_caps_free(p);
            }
        };
        using psram_ptr_t = std::unique_ptr<uint8_t, PSRAMDeleter<uint8_t>>;

    public:
        // 転送データパケットバッファ
        data_trans_packet_declare_buffer declare_buff;
        data_trans_packet_content_buffer content_buff;
        data_trans_packet_response_buffer resp_buff;
        // 転送データバッファ
        psram_ptr_t data_buff;
        size_t data_buff_size;
        bool data_buff_cmpl;
        // エラーメッセージ
        static constexpr size_t err_msg_len = 128;
        char err_msg[err_msg_len];

        // 受信ステータス
        uint32_t recv_page; // 受信済みページ数
        uint32_t data_recv_size;

    public:
        data_trans();
        ~data_trans();

        bool setup(BLEServer *server) override;

        void onWrite(BLECharacteristic *pCharacteristic) override;
        void onRead(BLECharacteristic *pCharacteristic) override;
        void onNotify(BLECharacteristic *pCharacteristic) override;

        void start() override;
        void stop() override;

        void notify();

        // ポーリング処理
        void polling(uint32_t cycle_ms);

        // データ取得
        bool has_data() const { return data_buff_cmpl; }
        psram_ptr_t &&get_buff()
        {
            data_buff_size = 0;
            data_buff_cmpl = false;
            return std::move(data_buff);
        }

    private:
        service_state state_;
        data_trans_state dt_state_;

        void analyze_rx(uint8_t *buff, size_t len);
        void analyze_rx_init(uint8_t *buff, size_t len);
        void analyze_rx_data_recving(uint8_t *buff, size_t len);

        void notify_declare_ng(data_trans_resp_reject_reason reason);
        void notify_declare_ok();
        void notify_content_ng(data_trans_resp_reject_reason reason);
        void notify_content_ok();
        void notify_data_complete();

        void init_state();

        void reset_timeout();
    };
}
