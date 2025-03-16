#pragma once

namespace lib_mini_appfx
{
    enum class event
    {
        None,

        // ページ変更
        // menu
        // app
        AppChange, // 指定のページに移動する, 直前に開いていたページを記憶せずに新しいページを開く
        AppPopup,  // 指定のページを重ねて表示する, 直前に開いていたページに戻れるようにする
        AppUpdate, // 表示更新要求
    };
}
