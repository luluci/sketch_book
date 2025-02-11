#pragma once

#include "defines.hpp"
#include "service/service_base.hpp"

#include <BLEDevice.h>
#include <BLEUtils.h>
#include <BLEServer.h>
#include <BLE2902.h>

#include <memory>
#include <string>
#include <vector>

namespace ble
{
    // BLEではPeripheral==Serverになる。
    class server : public BLEServerCallbacks, public BLESecurityCallbacks
    {
        // デバイス名
        std::string device_name_;
        // Server
        // BLEServerは1つのインスタンスを使いまわす
        // 解放せずに保持し続けるため生ポインタで保持
        BLEServer *server_ = nullptr;
        BLESecurity *security_ = nullptr;
        BLEAdvertising *advertising_ = nullptr;
        // Services
        std::vector<service::service_base *> services_;

    public:
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
        esp_bd_addr_t device_addr;
        char device_addr_str[ESP_BD_ADDR_LEN + 1];

    private:
        // コピー禁止
        server(server const &) = delete;
        server &operator=(server const &) = delete;

        server();

    public:
        ~server();

        // BLEはH/W的にも一つしかないためSingletonで管理する
        static server &get_instance()
        {
            static server instance;
            return instance;
        }

    public:
        // add_service -> setup -> startの順で呼び出す
        void add_service(service::service_base *service) { services_.push_back(service); }
        void setup(char const *device_name);
        void start();

        // Impl BLEServerCallbacks
        void onConnect(BLEServer *);
        void onDisconnect(BLEServer *);
        // Impl BLESecurityCallbacks
        uint32_t onPassKeyRequest();
        void onPassKeyNotify(uint32_t pass_key);
        bool onSecurityRequest();
        void onAuthenticationComplete(esp_ble_auth_cmpl_t);
        bool onConfirmPIN(uint32_t pin);

    private:
        size_t setup_services();
        void add_service_uuid_for_advertising();
        void start_services();
        void stop_services();
    };

}
