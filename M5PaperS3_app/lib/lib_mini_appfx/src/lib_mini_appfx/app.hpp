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
    public:
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

    private:
        // 表示app変更あり
        bool has_app_change_;

    public:
        bool is_build_ok;

    public:
        // pageのポインタを管理する
        template <typename... Args>
        app(Args &...args) : pages{(reinterpret_cast<page_type *>(&args))...}, request_queue(10), is_build_ok(false)
        {
            // 基底クラス、派生クラスの関係でコンストラクト後に初期設定を実施する
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
            // page設定チェック
            // コンストラクタで全ページ要素をセットする？
            if ((size_t)id_type::MAX != pages.size())
            {
                // throw std::runtime_error("required: pages item set to app ctor.");
                return;
            }
            for (size_t i = 0; i < pages.size(); i++)
            {
                auto &page = pages[i];
                if ((size_t)page->id != i)
                {
                    // throw std::runtime_error("required: pages-item in app ctor is ordered as enum.");
                    return;
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

            // 初期表示ページを設定する
            // menu
            if (menu_id < id_type::MAX)
            {
                menu_page = pages[menu_id];
                menu_page->is_active = true;
            }
            else
            {
                menu_page = nullptr;
            }
            // app
            if (app_id < id_type::MAX)
            {
                app_page = pages[app_id];
                app_page_layer.push_back(app_page);
                app_page->is_active = true;
            }
            else
            {
                app_page = nullptr;
            }

            //
            refresh();

            is_build_ok = true;
        }
        void refresh()
        {
            begin_render();
            if (app_page != nullptr)
            {
                app_page->render(true, 0);
            }
            if (menu_page != nullptr)
            {
                menu_page->render(true, 0);
            }
            end_render();
        }

        // ページタッチ判定
        // release -> press と変化した
        bool check_touch_pressed(int x_, int y_)
        {
            // 表示中の要素に対してタッチ判定を実施する
            // 優先度の高い順にチェックしてマッチした時点で終了する
            // check: menu
            if (check_touch_pressed_impl(menu_page, x_, y_))
            {
                return true;
            }
            // check: app
            if (check_touch_pressed_impl(app_page, x_, y_))
            {
                return true;
            }

            return false;
        }
        bool check_touch_pressed_impl(page_type *page, int x_, int y_)
        {
            if (page != nullptr)
            {
                if (page->check_touch_pressed(x_, y_))
                {
                    // リクエストを実行する
                    exec_request();
                    // イベントハンドラでは、イベント受理時にpage更新をチェックする
                    page->render(false, 0);
                    return true;
                }
            }
            return false;
        }

        // ページタッチ判定
        // pressを継続している
        bool check_touch_dragging(int x_, int y_, int delta_x_, int delta_y_)
        {
            // 表示中の要素に対してタッチ判定を実施する
            // 優先度の高い順にチェックしてマッチした時点で終了する
            // check: menu
            if (check_touch_dragging_impl(menu_page, x_, y_, delta_x_, delta_y_))
            {
                return true;
            }
            // check: app
            if (check_touch_dragging_impl(app_page, x_, y_, delta_x_, delta_y_))
            {
                return true;
            }

            return false;
        }
        bool check_touch_dragging_impl(page_type *page, int x_, int y_, int delta_x_, int delta_y_)
        {
            if (page != nullptr)
            {
                if (page->check_touch_dragging(x_, y_, delta_x_, delta_y_))
                {
                    // リクエストを実行する
                    exec_request();
                    // イベントハンドラでは、イベント受理時にpage更新をチェックする
                    page->render(false, 0);
                    return true;
                }
            }
            return false;
        }

        // ページタッチ判定
        // press -> release と変化した
        bool check_touch_released(int x_, int y_)
        {
            // 表示中の要素に対してタッチ判定を実施する
            // 優先度の高い順にチェックしてマッチした時点で終了する
            // check: menu
            if (check_touch_released_impl(menu_page, x_, y_))
            {
                return true;
            }
            // check: app
            if (check_touch_released_impl(app_page, x_, y_))
            {
                return true;
            }

            return false;
        }
        bool check_touch_released_impl(page_type *page, int x_, int y_)
        {
            if (page != nullptr)
            {
                if (page->check_touch_released(x_, y_))
                {
                    // リクエストを実行する
                    exec_request();
                    // イベントハンドラでは、イベント受理時にpage更新をチェックする
                    page->render(false, 0);
                    return true;
                }
            }
            return false;
        }

        void polling()
        {
            // 全ページに対してポーリング処理を適用する
            for (size_t i = 0; i < pages.size(); i++)
            {
                auto &page = pages[i];
                check_polling(page);
            }
            // 表示中のpageに対して更新チェックを実施する
            if (app_page != nullptr)
            {
                app_page->render(false, 0);
            }
            if (menu_page != nullptr)
            {
                menu_page->render(false, 0);
            }
        }
        void check_polling(page_type *page)
        {
            if (page != nullptr)
            {
                if (page->check_polling())
                {
                    // ポーリングによりリクエストが発生したら処理する
                    exec_request();
                }
            }
        }

        void exec_request()
        {
            if (request_queue.size() > 0)
            {
                has_app_change_ = false;

                begin_render();
                // pageからのリクエストを実行
                for (auto &req : request_queue)
                {
                    exec_request_impl(req);
                }
                // 表示app変更ありをmenuに通知する
                if (has_app_change_)
                {
                    on_change_app(app_page->id);
                }
                end_render();
                request_queue.clear();
            }
        }
        void exec_request_impl(request_type &req)
        {
            // 対象ページ取得
            // イベント処理
            switch (req.event)
            {
            case event::AppChange:
            {
                auto tgt_page = get_page(req.page_id);
                if (tgt_page != nullptr)
                {
                    // 現在表示中appを外す
                    app_page->is_active = false;
                    app_page_layer.pop_back();
                    // 新しいappを有効化
                    app_page = tgt_page;
                    app_page->is_active = true;
                    app_page_layer.push_back(app_page);
                    app_page->render(true, req.data);
                    has_app_change_ = true;
                }
                break;
            }
            case event::AppPopup:
            {
                auto tgt_page = get_page(req.page_id);
                if (tgt_page != nullptr)
                {
                    // 現在表示中appを無効化
                    app_page->is_active = false;
                    // 新しいappを有効化
                    app_page = tgt_page;
                    app_page->is_active = true;
                    app_page_layer.push_back(app_page);
                    app_page->render(true, req.data);
                    has_app_change_ = true;
                }
                break;
            }
            case event::AppClosePopup:
                if (app_page_layer.size() > 1)
                {
                    // 現在表示中appを外す
                    app_page->is_active = false;
                    app_page_layer.pop_back();
                    // 新しいappを有効化
                    app_page = app_page_layer.back();
                    app_page->is_active = true;
                    app_page->render(true, req.data);
                    has_app_change_ = true;
                }
                break;
            case event::AppUpdate:
                app_page->render(true, req.data);
                break;

            default:
                // nothing
                break;
            }
        }

        page_type *get_page(id_type page_id)
        {
            // 範囲チェック
            if (page_id < pages.size())
            {
                return pages[page_id];
            }
            // 無効IDではnullptrを返す
            return nullptr;
        }

        virtual void begin_render() {}
        virtual void end_render() {}
        // 表示app変更通知
        virtual void on_change_app(id_type new_app) = 0;
    };
}
