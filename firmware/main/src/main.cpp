#include <stdio.h>
#include "pico/stdlib.h"

#include "bsp/board_api.h"
#include "tusb.h"

#include "i2c/i2c_driver.h"
#include "uart/uart_driver.h"
#include "defs/defs.hpp"

using namespace drivers::i2c;
using namespace drivers::uart;

static constexpr uint8_t led = 2;


int main() {
    tusb_rhport_init_t dev_init = {
        .role = TUSB_ROLE_DEVICE,
        .speed = TUSB_SPEED_AUTO
    };
    tusb_init(BOARD_TUD_RHPORT, &dev_init);

    i2c_driver main_i2c(defs::i2c::inst, defs::i2c::sda, defs::i2c::scl, defs::i2c::slave_address, defs::i2c::baudrate);
    uart_driver main_uart(defs::uart::inst, defs::uart::rx, defs::uart::tx, defs::uart::baudrate);

    gpio_init(led);
    gpio_set_dir(led, true);
    gpio_put(led, 0);
    sleep_ms(10000);

    while (true) {
        tud_task();
    }
}
