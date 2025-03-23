#include "app_test_epdiy.hpp"

#include <lib_mini_appfx/event.hpp>
#include <tuple>

namespace app::pages
{
    app_test_epdiy::app_test_epdiy(id_type id_)
        : base_type::page(id_),
          test_disp_panel(component_id::TestDispPanel),
          navi_panel(component_id::NavigatorPanel),
          prev_button(component_id::PrevButton),
          next_button(component_id::NextButton)
    {
        // 表示領域
        set_coord(0, 80, 540, 880);

        // テスト表示パネル
        add(test_disp_panel);
        test_disp_panel.set_hit(false);
        test_disp_panel.set_coord(x, y, w, 780);

        // ナビゲーター
        add(navi_panel);
        navi_panel.set_hit(false);
        navi_panel.set_coord(x, 860, w, 100);
        // ボタン
        add(prev_button);
        prev_button.set_coord(20, 870, 120, 80);
        prev_button.set_text("<<Prev");
        prev_button.set_font(resrc.font_menu);
        prev_button.set_bkcolor(resrc.grayscale_13);
        add(next_button);
        next_button.set_coord(540 - 120 - 20, 870, 120, 80);
        next_button.set_text("Next>>");
        next_button.set_font(resrc.font_menu);
        next_button.set_bkcolor(resrc.grayscale_13);
    }
    bool app_test_epdiy::check_polling()
    {
        return false;
    }
    bool app_test_epdiy::on_touch_released(component_type *, int, int)
    {
        return false;
    }
}
