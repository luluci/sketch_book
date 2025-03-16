#pragma once

#include <cstdint>

#include "./event.hpp"

namespace lib_mini_appfx
{
    // pageからappに発行するリクエスト
    template <typename PageId>
    struct page_request
    {
        lib_mini_appfx::event event; // 発行イベント
        PageId page_id;              // ページ情報
        uint32_t data;               // app付加情報

        page_request() : event(lib_mini_appfx::event::None), page_id(PageId::MAX), data(0) {}
        page_request(lib_mini_appfx::event event_, PageId page_id_, uint32_t data_) : event(event_), page_id(page_id_), data(data_) {}
    };
}
