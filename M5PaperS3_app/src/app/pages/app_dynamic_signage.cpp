#include "app_dynamic_signage.hpp"

#include <lib_mini_appfx/event.hpp>
#include <tuple>

namespace app::pages
{
    app_dynamic_signage::app_dynamic_signage(id_type id_)
        : base_type::page(id_),
          panel(component_id::Panel),
          label_dest(component_id::LabelDest),
          label_depart(component_id::LabelDepart),
          label_name(component_id::LabelName)
    {
        // app領域
        set_coord(0, 0, 880, 540);

        //
        add(panel);
        panel.set_hit(false);
        panel.set_coord(x, y, w, h);

        // 行き先表示
        add(label_dest);
        label_dest.set_coord(x, y, w, 270);
        label_dest.set_font(resrc.font_signage);
        label_dest.set_text_size(3);
        label_dest.set_align(components::label_base::text_alignment::Center);
        label_dest.set_bkcolor(resrc.grayscale_14);
        label_dest.set_text("付加情報表示");
        // 部署名表示
        add(label_depart);
        label_depart.set_coord(x, y + 270, 300, 270);
        label_depart.set_font(resrc.font_signage);
        label_depart.set_text_size(2);
        label_depart.set_align(components::label_base::text_alignment::Center);
        label_depart.set_bkcolor(resrc.grayscale_16);
        label_depart.set_text("部署名表示");
        // 名前表示
        add(label_name);
        label_name.set_coord(x + 300, y + 270, 580, 270);
        label_name.set_font(resrc.font_signage);
        label_name.set_text_size(2);
        label_name.set_align(components::label_base::text_alignment::Center);
        label_name.set_bkcolor(resrc.grayscale_16);
        label_name.set_text("名前表示");
    }
    void app_dynamic_signage::begin_render(uint32_t data)
    {
        rotate_org_ = M5.Display.getRotation();
        M5.Display.setRotation(1);
    }
    void app_dynamic_signage::end_render(uint32_t data)
    {
        M5.Display.setRotation(rotate_org_);
    }
    bool app_dynamic_signage::check_polling()
    {
        return false;
    }
    bool app_dynamic_signage::on_touch_released(component_type *, int, int)
    {
        return false;
    }
}
