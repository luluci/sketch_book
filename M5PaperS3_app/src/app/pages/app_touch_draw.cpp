#include "app_touch_draw.hpp"

#include <lib_mini_appfx/event.hpp>
#include <tuple>

namespace app::pages
{
    app_touch_draw::app_touch_draw(id_type id_)
        : base_type::page(id_), canvas(component_id::Canvas), clear_button(component_id::ClearButton)
    {
        // ポップアップ領域
        set_coord(0, 80, 540, 880);

        // canvas
        add(canvas);
        canvas.set_coord(x, y, w, canvas_h);
        // button
        add(clear_button);
        clear_button.set_coord(x, y + canvas_h, 120, button_h);
        clear_button.set_text("CLEAR");
        clear_button.set_font(resrc.font_menu);
        clear_button.set_bkcolor(resrc.grayscale_4);
    }
    bool app_touch_draw::check_polling()
    {
        return false;
    }
    bool app_touch_draw::on_touch_released(component_type *component, int x_, int y_)
    {
        if (component->id == canvas.id)
        {
            canvas.on_touch_released(x_, y_);
        }
        if (component->id == clear_button.id)
        {
            canvas.clear();
        }
        return false;
    }
}