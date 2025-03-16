#pragma once

#include <vector>

#include "./page.hpp"
#include "./event.hpp"
#include "./request.hpp"

namespace lib_mini_appfx
{
    // applicationベース
    template <typename Id>
    class app
    {
        // Idはenumによる連番で指定する
        // Idは最大要素数を示すMAXを持つ必要がある
        using id_type = Id;
        static_assert(std::is_enum<id_type>::value == true, "required: Id is enum");
        //
        using page_type = page_base<id_type>;

        // ページコンテナ
        std::vector<page_type *> pages;
        // 表示ページ管理情報
        // 常時表示するmenuと、ツールごとに表示するappで構成する
        // menuはpageから一つを選択する
        page_type *menu_page;
        // appは「前ページに戻る」のためにレイヤーを記憶する
        std::vector<page_type *> app_page_layer;
        page_type *app_page;

        // pageからappへのリクエストキュー
        using request_type = page_request<Id>;
        using request_container_type = std::vector<request_type>;
        request_container_type request_queue;

    public:
        // pageのポインタを管理する
        template <typename... Args>
        app(Args &...args) : pages{(reinterpret_cast<page_type *>(&args))...}, request_queue(10)
        {
            // page設定チェック
            // コンストラクタで全ページ要素をセットする？
            if ((size_t)id_type::MAX != pages.size())
            {
                throw std::runtime_error("required: pages item set to app ctor.");
            }
            for (size_t i = 0; i < pages.size(); i++)
            {
                auto &page = pages[i];
                if ((size_t)page->id != i)
                {
                    throw std::runtime_error("required: pages-item in app ctor is ordered as enum.");
                }
                //
                page->set_request_queue(&request_queue);
            }

            // // 初期化
            // if (pages.size() > 0)
            // {
            //     curr_page = pages[page_idx];
            // }
            // else
            // {
            //     // ありえない
            // }
        }
        // pageインスタンスは保持しない
        template <typename... Args>
        app(Args &&...args) = delete;
        // 後からpage追加は対応しない？
        // void add_page(page_type &p)
        // {
        //     pages.push_back(&p);
        // }
        // void add_page(page_type &&p) {}

        void init(id_type menu_id, id_type app_id)
        {
            menu_page = pages[menu_id];

            app_page = pages[app_id];
            app_page_layer.push_back(app_page);
        }

        // ページタッチ判定
        bool check_click(int x_, int y_)
        {
            // 表示中の要素に対してタッチ判定を実施する
            // 優先度の高い順にチェックしてマッチした時点で終了する
            // check: menu
            if (check_click_impl(menu_page, x_, y_))
            {

                return true;
            }
            // check: app
            if (check_click_impl(app_page, x_, y_))
            {
                return true;
            }

            return false;
        }
        bool check_click_impl(page_type *page, int x_, int y_)
        {
            if (page != nullptr)
            {
                if (page->check_click(x_, y_))
                {
                    exec_request();
                    return true;
                }
            }
            return false;
        }

        void polling()
        {
            check_polling(menu_page);
            check_polling(app_page);
        }
        void check_polling(page_type *page)
        {
            if (page != nullptr)
            {
                if (page->check_polling())
                {
                    exec_request();
                }
            }
        }

        void exec_request()
        {
            for (auto &req : request_queue)
            {
                exec_request_impl(req);
            }
            request_queue.clear();
        }
        void exec_request_impl(request_type &req)
        {
            // 対象ページ取得
            // failsafe
            if (req.page_id >= pages.size())
            {
                return;
            }
            app_page = pages[req.page_id];
            // イベント処理
            switch (req.event)
            {
            case event::AppChange:
                app_page_layer.pop_back();
                app_page_layer.push_back(app_page);
                app_page->render(true, req.data);
                break;
            case event::AppPopup:
                app_page_layer.push_back(app_page);
                app_page->render(true, req.data);
                break;
            case event::AppUpdate:
                app_page->render(false, req.data);
                break;

            default:
                // nothing
                break;
            }
        }
    };
}
