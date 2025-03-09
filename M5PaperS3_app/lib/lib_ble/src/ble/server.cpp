#include "server.hpp"

namespace ble
{
    server::server() : device_addr{'1', '1', '1', '9', '9', '9'}, device_addr_str{0}
    {
    }

    server::~server()
    {
    }

    void server::setup(char const *device_name)
    {
        // Serverを初期化する
        state_ = server_state::Init;

        device_name_ = device_name;
        //
        BLEDevice::init(device_name_);
        // BLEDevice::setEncryptionLevel(ESP_BLE_SEC_ENCRYPT);
        BLEDevice::setSecurityCallbacks(this);
        // Server
        server_ = BLEDevice::createServer();
        server_->setCallbacks(this);

        // // Security
        // security_ = new BLESecurity();
        // security_->setAuthenticationMode(ESP_LE_AUTH_REQ_SC_BOND); // ESP_LE_AUTH_REQ_SC_ONLY ESP_LE_AUTH_REQ_SC_MITM_BOND
        // security_->setCapability(ESP_IO_CAP_NONE);                 // ESP_IO_CAP_KBDISP
        // security_->setInitEncryptionKey(ESP_BLE_ENC_KEY_MASK | ESP_BLE_ID_KEY_MASK);
        // security_->setStaticPIN(111999);

        // Services setup
        active_service_count = setup_services();
    }
    void server::start()
    {
        // 初期状態からAdvertisingを開始する
        if (state_ != server_state::Init)
        {
            return;
        }

        // Advertising設定
        advertising_ = server_->getAdvertising();
        add_service_uuid_for_advertising();
        advertising_->setDeviceAddress(device_addr, BLE_ADDR_TYPE_RANDOM);
        advertising_->setScanResponse(true);
        // advertising_->setScanFilter(false, true);

        // iPhoneの接続問題への対処らしい
        advertising_->setMinPreferred(0x06);
        advertising_->setMinPreferred(0x12);

        // 先にserviceを起動する？
        start_services();
        state_ = server_state::Advertising;

        // Advertising開始
        advertising_->start();
        // BLEDevice::startAdvertising();
    }

    void server::stop()
    {
        // 初期状態では処理なし
        if (state_ == server_state::Init)
        {
            return;
        }

        // Advertising中なら停止
        if (state_ == server_state::Advertising)
        {
            advertising_->stop();
        }

        // 初期状態でなければService稼働中なので停止する
        stop_services();

        //
        state_ = server_state::Init;
    }

    void server::onConnect(BLEServer *)
    {
        // 接続通知
        advertising_->stop();
        // BLEDevice::stopAdvertising();
        state_ = server_state::Connected;
        event_ |= server_event::onConnect;
    }

    void server::onDisconnect(BLEServer *)
    {
        // 切断通知
        // Advertising再開
        advertising_->start();
        // BLEDevice::startAdvertising();
        state_ = server_state::Advertising;
        event_ |= server_event::onDisconnect;
    }

    uint32_t server::onPassKeyRequest()
    {
        event_ |= server_event::onPassKeyRequest;
        return pass_key_;
    }

    void server::onPassKeyNotify(uint32_t pass_key)
    {
        event_ |= server_event::onPassKeyNotify;
        pass_key_ = pass_key;
    }

    bool server::onSecurityRequest()
    {
        event_ |= server_event::onSecurityRequest;
        return true;
    }

    void server::onAuthenticationComplete(esp_ble_auth_cmpl_t auth_cmpl)
    {
        event_ |= server_event::onAuthenticationComplete;
        auth_cmpl_ = auth_cmpl;
    }

    bool server::onConfirmPIN(uint32_t pin)
    {
        event_ |= server_event::onConfirmPIN;
        return true;
    }

    size_t server::setup_services()
    {
        // tuple全要素に対してsetupを実行する
        // 各serviceはsetupに失敗しても内部的に無効化して、serverは動作継続
        size_t count = 0;
        for (auto &service : services_)
        {
            bool result = service->setup(server_);
            if (result)
            {
                count++;
            }
        }
        return count;
    }
    void server::add_service_uuid_for_advertising()
    {
        // 各serviceはsetupに失敗しても内部的に無効化して、serverは動作継続
        for (auto &service : services_)
        {
            advertising_->addServiceUUID(service->get_uuid());
        }
    }
    void server::start_services()
    {
        // 各serviceはsetupに失敗しても内部的に無効化して、serverは動作継続
        for (auto &service : services_)
        {
            service->start();
        }
    }
    void server::stop_services()
    {
        // 各serviceはsetupに失敗しても内部的に無効化して、serverは動作継続
        for (auto &service : services_)
        {
            service->stop();
        }
    }
}