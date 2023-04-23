#include "main.hpp"

#include "ir.hpp"
#include "serial.hpp"

void setup(void) {
    // config
    auto cfg = M5.config();
    cfg.serial_baudrate = 115200;
    // cfg.output_power = true;
    M5_BEGIN(cfg);

    // IR初期化
    ir_init();

    // LCD初期表示
    M5.Display.setTextSize(1);
    M5.Display.startWrite();
    M5.Display.println("Wait UART...");
    M5.Display.endWrite();
}

static constexpr size_t disp_buff_size = 255;
char disp_buff[disp_buff_size];

static constexpr size_t display_row_max = 16;
static constexpr size_t display_col_max = 21;

void loop(void) {
    // UART処理
    if (check_serial()) {
        // 受信取得
        auto len = read_serial();

        // 受信バイトシーケンスを表示
        M5.Display.startWrite();
        M5.Display.clear();
        M5.Display.setCursor(0, 0);
        {
            // 1行目：受信データ数
            snprintf(disp_buff, disp_buff_size, "Len: %d", len);
            M5.Display.println(disp_buff);
            // 2行目～：受信データHEX文字列
            // MAX16行？
            // 1行MAX22字？
            size_t buff_pos = 0;
            int disped_line = 0;
            while (buff_pos < len && disped_line < display_row_max) {
                // 1行作成
                size_t pos = 0;
                size_t disp_buff_pos = 0;
                while (buff_pos < len && disp_buff_pos < display_col_max &&
                       pos < 7) {
                    // 1バイト毎に区切文字を入れる
                    if (pos > 0) {
                        disp_buff[disp_buff_pos] = '-';
                        disp_buff_pos++;
                    }
                    //
                    hex2ascii(&disp_buff[disp_buff_pos], buff[buff_pos]);
                    disp_buff_pos += 2;
                    pos++;
                    buff_pos++;
                }
                disp_buff[disp_buff_pos] = '\0';
                // 1行表示
                M5.Display.println(disp_buff);
                disped_line++;
            }
        }
        M5.Display.endWrite();

        // 待機
        delay(500);
        // 受信データをそのまま送信
        write_serial();
    }

    // M5.update();

    delay(50);
}
