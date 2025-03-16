#pragma once

#include <type_traits>

namespace lib_mini_appfx
{
    template <typename Id>
    class component
    {
    public:
        using id_type = Id;
        id_type id;

        int x; // 始点:X座標
        int y; // 始点:Y座標
        int w;
        int h;
        int x2; // 終点:X座標
        int y2; // 終点:Y座標

        bool has_hit;

    public:
        component(id_type id_) : id(id_), has_hit(true) {}

        void set_coord(int x_, int y_, int w_, int h_)
        {
            x = x_;
            y = y_;
            w = w_;
            h = h_;
            x2 = x_ + w_;
            y2 = y_ + h_;
        }
        void set_hit(bool flag) { has_hit = flag; }
        auto width() { return w; }
        auto height() { return h; }

        bool hit_check(int x_, int y_)
        {
            if (has_hit)
            {
                // ヒット判定実行
                if (x <= x_ && x_ <= x2)
                {
                    if (y <= y_ && y_ <= y2)
                    {
                        return true;
                    }
                }

                return false;
            }
            else
            {
                return false;
            }
        }

        virtual void render() = 0;
    };
}
