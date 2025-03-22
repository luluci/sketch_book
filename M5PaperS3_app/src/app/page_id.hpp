#pragma once

namespace app
{

    enum page_id
    {
        // menu用ページ
        MenuHeader,
        MenuPopup, // app側に表示する

        // app用ページ
        AppMain,         // メイン画面
        AppPowerManager, // 電源管理画面
        AppTestEPDiy,    // EPDiyテスト
        AppDrawCanvas,   //
        AppBLE,          // BLE管理

        MAX
    };
}
