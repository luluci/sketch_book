#include "main.hpp"

#include <Wire.h>

#include "ir.hpp"
#include "menu.hpp"
#include "serial.hpp"

static constexpr size_t disp_buff_size = 255;
char disp_buff[disp_buff_size];

static constexpr size_t i2c_buff_size = 255;
uint8_t i2c_buff[i2c_buff_size];

static constexpr size_t display_row_max = 16;
static constexpr size_t display_col_max = 21;

static uint8_t led_state = 0;

namespace my_menu {
enum class popup_menu
{
    progress,
    alert,
};

struct popup_progress
{
    lib_menu::response_t on_entry() {
        return lib_menu::response_t::OK;
    }
    lib_menu::response_t on_confirm(size_t index) {
        return lib_menu::response_t::ExitPopup;
    }
    void on_render() {
        std::cout << "popup_progress" << std::endl;
    }
};

struct data_node
{
    char const *label;
    size_t len;

    static constexpr char charno_data_label[] = "<no data>";
    static constexpr size_t no_data_label_len = sizeof(charno_data_label);

    data_node() : label(nullptr), len(0) {
    }
    data_node(char const *str) : label(str), len(strlen(str)) {
    }

    operator bool() {
        return label != nullptr;
    }

    void get_label(char *buffer, size_t buff_size) {
        if (label != nullptr) {
            std::memcpy(buffer, label, std::min(len + 1, buff_size));
        } else {
            std::memcpy(buffer, charno_data_label,
                        std::min(no_data_label_len, buff_size));
        }
    }
};

data_node data_c[] = {
    data_node{"item1"},
    data_node{},
    data_node{"item3"},
};

struct header_root_menu
{
    void operator()(void *data) {
        std::cout << "<header_root_menu>" << std::endl;
    }
};
struct header_menu_1
{
    void operator()(void *data) {
        std::cout << "<header_menu_1>" << std::endl;
    }
};

struct actor_back
{
    /*
    actor_back(){
            // ctor
    }
    actor_back(actor_back const& obj){
            std::cout << "copy ctor" << std::endl;
    }
    actor_back(actor_back &&obj)
    {
            std::cout << "move ctor" << std::endl;
    }
    */

    lib_menu::response_t on_entry() {
        return lib_menu::response_t::ReqBack;
    }
    lib_menu::response_t on_confirm(size_t index) {
        return lib_menu::response_t::ReqBack;
    }
};

struct my_buffer_interface
{
    static constexpr size_t BufferRowMax = lib_menu::BufferRowMax;
    static constexpr size_t BufferColMax = lib_menu::BufferColMax;

    char buffer[lib_menu::BufferRowMax][lib_menu::BufferColMax];
    size_t pos;

    my_buffer_interface() : buffer{0}, pos{0} {
    }
};
my_buffer_interface buffer;

using lm = lib_menu::builder<my_buffer_interface, popup_menu>;

auto menu_back = lm::leaf("[戻る]", actor_back{});

// clang-format off
	auto menu_mng = lm::make_menu(
		buffer,
		lm::header(header_root_menu()),

		lm::node("menu_1",
			lm::header(header_menu_1()),

			lm::node(data_c,
				lm::leaf("dyn_menu_1", actor_back{}),
				lm::leaf("dyn_menu_2", actor_back{}),
				lm::leaf("dyn_menu_3", actor_back{}),
				menu_back
			),
			lm::leaf("menu_1_2", actor_back{}),
			menu_back
		),

		lm::node("menu_2",
			lm::leaf("menu_2_1", actor_back{}),
			lm::leaf("menu_2_2", actor_back{}),
			lm::leaf("menu_2_3", actor_back{}),
			menu_back
		),

		lm::node("menu_3",
			lm::node("menu_3_1",
				lm::leaf("menu_3_1_1", actor_back{}),
				lm::node("menu_3_1_2",
					lm::leaf("menu_3_1_2_1", actor_back{}),
					lm::leaf("menu_3_1_2_2", actor_back{}),
					lm::leaf("menu_3_1_2_3", actor_back{}),
					menu_back
				),
				lm::leaf("menu_3_1_3", actor_back{}),
				menu_back
			),
			lm::leaf("menu_3_2", actor_back{}),
			menu_back
		),

		lm::leaf("test", actor_back{}),
		lm::leaf("Version", actor_back{})
	).add(lm::popup(popup_menu::progress, popup_progress{}));
// clang-format on

void event_next() {
    std::cout << "[event:[SelectNext]]" << std::endl;
    menu_mng.on_select_next();
}
void event_prev() {
    std::cout << "[event:[SelectPrev]]" << std::endl;
    menu_mng.on_select_prev();
}
void event_confirm() {
    std::cout << "[event:[Confirm]]" << std::endl;
    menu_mng.on_confirm();
}
void event_popup(popup_menu tgt) {
    menu_mng.on_popup(tgt);
}
}  // namespace my_menu

void setup(void) {
    // config
    auto cfg = M5.config();
    cfg.serial_baudrate = 115200;
    // cfg.output_power = true;
    M5_BEGIN(cfg);

    my_menu::menu_mng.init();

    // IR初期化
    ir_init();
    // pinMode(4, OUTPUT);
    // digitalWrite(4, led_state);

    // I2C初期化
    serial::buff.frame.i2c_addr = 0x23;
    serial::buff.frame.i2c_reg = 0x02;
    //
    if (false) {
        M5.Ex_I2C.begin(0, 2, 1);
    }
    if (true) {
        uint32_t freq = 40 * 1000;
        Wire.begin(M5.Ex_I2C.getSDA(), M5.Ex_I2C.getSCL(), freq);
    }
    // LCD初期表示
    M5.Display.setTextSize(1);
    M5.Display.startWrite();
    M5.Display.println("Wait UART...");
    M5.Display.endWrite();
}

void loop(void) {
    // button
    if (M5.BtnA.wasPressed()) {
        // while (M5.BtnA.isPressed()) {
        //     delay(50);
        // }
        //
        led_state = 1 - led_state;
        // digitalWrite(4, led_state);
        ir_send(serial::buff.bytes, serial::buff_pos);
        //
        M5.Display.startWrite();
        M5.Display.clear();
        M5.Display.setCursor(0, 0);
        snprintf(disp_buff, disp_buff_size, "Ir LED: %d", led_state);
        M5.Display.println(disp_buff);
        // M5.Display.setTextColor(0, 0x00FFFFFF);
        M5.Display.endWrite();

        // I2C master動作を実行する
        if (true) {
            snprintf(disp_buff, disp_buff_size, "I2C: ADDR=%02X,REG=%02X",
                     serial::buff.frame.i2c_addr, serial::buff.frame.i2c_reg);
            M5.Display.println(disp_buff);
            // address: 0xAB
            // register: 0x02 から 2byte read
            // 40kHz
            uint32_t freq = 40 * 1000;
            if (true) {
                bool result = true;
                size_t status;
                Wire.beginTransmission(serial::buff.frame.i2c_addr);
                if (result) {
                    status = Wire.write(serial::buff.frame.i2c_reg);
                    result = status == 1;
                    if (!result) {
                        // 通信失敗
                        M5.Display.println("  failed: WRITE");
                    }
                }
                if (result) {
                    status = Wire.endTransmission(false);
                    result = status == 0;
                    if (!result) {
                        // 通信失敗
                        M5.Display.println("  failed: WRITE");
                    }
                }
                if (result) {
                    status = Wire.requestFrom(serial::buff.frame.i2c_addr,
                                              (uint8_t)2);
                    result = status == 2;
                    if (!result) {
                        // 通信失敗
                        M5.Display.println("  failed: READ");
                    }
                }
                if (result) {
                    status = Wire.readBytes(i2c_buff, 2);
                    result = status == 2;
                    if (!result) {
                        // 通信失敗
                        M5.Display.println("  failed: READ");
                    }
                }
                if (result) {
                    // 通信成功
                    snprintf(disp_buff, disp_buff_size, "  recv: 0x%02X 0x%02X",
                             i2c_buff[0], i2c_buff[1]);
                    M5.Display.println(disp_buff);
                }

                Wire.flush();
            }
            if (false) {
                bool result = true;
                result = M5.Ex_I2C.readRegister(serial::buff.frame.i2c_addr,
                                                serial::buff.frame.i2c_reg,
                                                i2c_buff, 2, 40 * 1000);
                if (result) {
                    // 通信成功
                    snprintf(disp_buff, disp_buff_size, "  recv: 0x%02X 0x%02X",
                             i2c_buff[0], i2c_buff[1]);
                    M5.Display.println(disp_buff);
                } else {
                    // 通信失敗
                    M5.Display.println("  failed");
                }
            }
            if (false) {
                bool result = true;
                if (result) {
                    result = M5.Ex_I2C.start(serial::buff.frame.i2c_addr, false,
                                             freq);
                    if (!result) {
                        // 通信失敗
                        M5.Display.println("  failed: START");
                    }
                }
                if (result) {
                    result = M5.Ex_I2C.write(serial::buff.frame.i2c_reg);
                    if (!result) {
                        // 通信失敗
                        M5.Display.println("  failed: WRITE");
                    }
                }
                if (result) {
                    result = M5.Ex_I2C.restart(serial::buff.frame.i2c_addr,
                                               true, freq);
                    if (!result) {
                        // 通信失敗
                        M5.Display.println("  failed: RESTART");
                    }
                }
                if (result) {
                    result = M5.Ex_I2C.read(i2c_buff, 2);
                    if (!result) {
                        // 通信失敗
                        M5.Display.println("  failed: READ");
                    } else {
                        // READだけ失敗したときに勝手にstopするっぽい
                        // READ以外は成功失敗に関わらずstopしないっぽい
                        M5.Ex_I2C.stop();
                    }
                } else {
                    M5.Ex_I2C.stop();
                }
                // if (result) {
                //     result = M5.Ex_I2C.stop();
                //     if (!result) {
                //         // 通信失敗
                //         M5.Display.println("  failed: STOP");
                //     }
                // }
                if (result) {
                    // 通信成功
                    snprintf(disp_buff, disp_buff_size, "  recv: 0x%02X 0x%02X",
                             i2c_buff[0], i2c_buff[1]);
                    M5.Display.println(disp_buff);
                }
            }
        }
    }
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
                    hex2ascii(&disp_buff[disp_buff_pos],
                              serial::buff.bytes[buff_pos]);
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

    delay(50);
    M5_UPDATE();
}
