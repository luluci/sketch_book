#pragma once

#include <vector>

#include "./component.hpp"
#include "./event.hpp"
#include "./request.hpp"

namespace lib_mini_appfx
{
    // pageベース
    template <typename Id>
    class page_base
    {
    public:
        using id_type = Id;
        const id_type id;

        // componentに対するoffsetになる
        int x; // 始点:X座標
        int y; // 始点:Y座標
        int w;
        int h;
        int x2; // 終点:X座標
        int y2; // 終点:Y座標
        // active状態
        bool is_active;

        // pageからappへのリクエストキューポインタ, インスタンスはappが管理
        using request_type = page_request<Id>;
        using request_container_type = std::vector<request_type>;
        request_container_type *request_queue;

        //
        void req(event ev, id_type id_, uint32_t data) { request_queue->emplace_back(ev, id_, data); }
        void req_change(id_type id_, uint32_t data) { request_queue->emplace_back(event::AppChange, id_, data); }
        void req_popup(id_type id_, uint32_t data) { request_queue->emplace_back(event::AppPopup, id_, data); }
        void req_close_popup(uint32_t data) { request_queue->emplace_back(event::AppClosePopup, id_type::MAX, data); }

    public:
        page_base(id_type id_) : id(id_), is_active(false), request_queue(nullptr) {}

        void set_request_queue(request_container_type *q) { request_queue = q; }
        void set_coord(int x_, int y_, int w_, int h_)
        {
            x = x_;
            y = y_;
            w = w_;
            h = h_;
            x2 = x_ + w_ - 1;
            y2 = y_ + h_ - 1;
        }

        // 周期処理判定
        virtual bool check_polling() = 0;
        // ページタッチ判定
        virtual bool check_touch_pressed(int x_, int y_) = 0;
        virtual bool check_touch_dragging(int x_, int y_, int delta_x_, int delta_y_) = 0;
        virtual bool check_touch_released(int x_, int y_) = 0;

        // ページ描画処理
        virtual void render(bool init, uint32_t data) = 0;
    };

    // pageベース
    template <typename Id, typename ComponentId>
    class page : public page_base<Id>
    {
    public:
        using page_base_type = page_base<Id>;
        using typename page_base_type::id_type;

        using component_type = component<ComponentId>;
        std::vector<component_type *> components;

    public:
        page(id_type id_) : page_base_type(id_), components()
        {
        }

        void add(component_type &component)
        {
            components.push_back(&component);
        }

        virtual bool check_touch_pressed(int x_, int y_) override
        {
            // 描画は0開始で上書きしていく
            // 判定チェックは逆順
            int i = components.size();
            do
            {
                i--;
                // 処理
                auto &comp = components[i];
                if (comp->hit_check(x_, y_))
                {
                    return on_touch_pressed(comp, x_, y_);
                }
            } while (i > 0);

            return false;
        }
        virtual bool on_touch_pressed(component_type *component, int x_, int y_) = 0;

        bool check_touch_dragging(int x_, int y_, int delta_x_, int delta_y_)
        {
            // 描画は0開始で上書きしていく
            // 判定チェックは逆順
            int i = components.size();
            do
            {
                i--;
                // 処理
                auto &comp = components[i];
                if (comp->hit_check(x_, y_))
                {
                    return on_touch_dragging(comp, x_, y_, delta_x_, delta_y_);
                }
            } while (i > 0);

            return false;
        }
        virtual bool on_touch_dragging(component_type *component, int x_, int y_, int delta_x_, int delta_y_) = 0;

        virtual bool check_touch_released(int x_, int y_) override
        {
            // for (int i = 0; i < components.size(); i++)
            // {
            //     auto &comp = components[i];
            //     if (comp->hit_check(x_, y_))
            //     {
            //         return on_touch(comp);
            //     }
            // }

            // 描画は0開始で上書きしていく
            // 判定チェックは逆順
            int i = components.size();
            do
            {
                i--;
                // 処理
                auto &comp = components[i];
                if (comp->hit_check(x_, y_))
                {
                    return on_touch_released(comp, x_, y_);
                }
            } while (i > 0);

            return false;
        }
        virtual bool on_touch_released(component_type *component, int x_, int y_) = 0;

        virtual void render(bool init, uint32_t data) override
        {
            begin_render(data);

            // 描画は0開始で上書きしていく
            for (int i = 0; i < components.size(); i++)
            {
                auto &comp = components[i];
                comp->render();
            }

            end_render(data);
        }
        virtual void begin_render(uint32_t) {}
        virtual void end_render(uint32_t) {}
    };
}
