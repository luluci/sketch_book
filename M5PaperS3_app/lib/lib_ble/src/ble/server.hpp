#pragma once

#include "defines.hpp"

#include <BLEDevice.h>
#include <BLEUtils.h>
#include <BLEServer.h>
#include <BLE2902.h>

#include <memory>
#include <string>
#include <tuple>

namespace ble
{
    // BLEではPeripheral==Serverになる。
    template <class... Services>
    class server : public BLEServerCallbacks
    {
        // SelfType
        using server_type = server<Services...>;

        // デバイス名
        std::string device_name_;
        // Server
        // BLEServerは1つのインスタンスを使いまわす
        // 解放せずに保持し続けるため生ポインタで保持
        BLEServer *server_ = nullptr;
        BLEAdvertising *advertising_ = nullptr;
        // Services
        std::tuple<Services...> services_;

    public:
        template <size_t I>
        auto &get_service()
        {
            return std::get<I>(services_);
        }

        bool is_connected()
        {
            return state_ == server_state::Connected;
        }

        char *get_device_addr_str()
        {
            for (size_t i = 0; i < ESP_BD_ADDR_LEN; i++)
            {
                device_addr_str[i] = device_addr[i];
            }
            device_addr_str[ESP_BD_ADDR_LEN] = '\0';
            return device_addr_str;
        }

    private:
        server_state state_;

    public:
        size_t active_service_count;
        static constexpr size_t service_count = sizeof...(Services);
        esp_bd_addr_t device_addr;
        char device_addr_str[ESP_BD_ADDR_LEN + 1];

    private:
        // コピー禁止
        server(server const &) = delete;
        server &operator=(server const &) = delete;

    public:
        server() : device_addr{'1', '1', '1', '9', '9', '9'} {}
        ~server() {}

        void setup(char const *device_name, Services &&...services)
        {
            // Serverを初期化する
            state_ = server_state::Init;

            device_name_ = device_name;
            //
            BLEDevice::init(device_name_);
            // Server
            server_ = BLEDevice::createServer();
            server_->setCallbacks(this);

            // Serviceを引数で受け取りtupleで管理する
            // Services
            services_ = std::forward_as_tuple(services...);
            auto failed_service_count = setup_services(services_);
            active_service_count = service_count - failed_service_count;
        }

        void start()
        {
            // 初期状態からAdvertisingを開始する
            if (state_ != server_state::Init)
            {
                return;
            }

            // Advertising開始
            advertising_ = server_->getAdvertising();
            add_service_uuid_for_advertising(services_);
            advertising_->setDeviceAddress(device_addr, BLE_ADDR_TYPE_RANDOM);
            advertising_->setScanResponse(true);

            // iPhoneの接続問題への対処らしい
            advertising_->setMinPreferred(0x06);
            advertising_->setMinPreferred(0x12);

            advertising_->start();
            // BLEDevice::startAdvertising();
            state_ = server_state::Advertising;
        }

        void onConnect(BLEServer *pServer)
        {
            // 接続通知
            advertising_->stop();
            // BLEDevice::stopAdvertising();
            state_ = server_state::Connected;
        }
        void onDisconnect(BLEServer *pServer)
        {
            // 切断通知
            // Advertising再開
            advertising_->start();
            // BLEDevice::startAdvertising();
            state_ = server_state::Advertising;
        }

    private:
        template <size_t I = 0, typename... Args>
        size_t setup_services(std::tuple<Args...> &services)
        {
            // tuple全要素に対してsetupを実行する
            // 各serviceはsetupに失敗しても内部的に無効化して、serverは動作継続
            if constexpr (I < service_count)
            {
                // setupを実行、失敗したらカウントする
                bool result = std::get<I>(services).setup(server_);
                size_t count = 0;
                if (!result)
                {
                    count++;
                }
                return setup_services<I + 1>(services) + count;
            }
            else
            {
                return 0;
            }
        }

        template <size_t I = 0, typename... Args>
        void add_service_uuid_for_advertising(std::tuple<Args...> &services)
        {
            // tuple全要素に対してsetupを実行する
            // 各serviceはsetupに失敗しても内部的に無効化して、serverは動作継続
            if constexpr (I < service_count)
            {
                advertising_->addServiceUUID(std::get<I>(services).get_uuid());
                add_service_uuid_for_advertising<I + 1>(services);
                return;
            }
            else
            {
                return;
            }
        }

        template <size_t I = 0, typename... Args>
        void start_services(std::tuple<Args...> &services)
        {
            // tuple全要素に対してsetupを実行する
            // 各serviceはsetupに失敗しても内部的に無効化して、serverは動作継続
            if constexpr (I < service_count)
            {
                std::get<I>(services).start();
                start_services<I + 1>(services);
                return;
            }
            else
            {
                return;
            }
        }
        template <size_t I = 0, typename... Args>
        void stop_services(std::tuple<Args...> &services)
        {
            // tuple全要素に対してsetupを実行する
            // 各serviceはsetupに失敗しても内部的に無効化して、serverは動作継続
            if constexpr (I < service_count)
            {
                std::get<I>(services).stop();
                stop_services<I + 1>(services);
                return;
            }
            else
            {
                return;
            }
        }
    };

}
