#include <stdio.h>
#include "pico/stdlib.h"

#include "bsp/board_api.h"

#include "i2c/i2c_driver.h"
#include "uart/uart_driver.h"
#include "gpio/gpio_driver.h"
#include "commands/commands.h"
#include "defs/defs.hpp"
#include "tinyusb.hpp"

using namespace drivers::i2c;
using namespace drivers::uart;
using namespace drivers::gpio;
using namespace lib::commands;

tiny_usb tusb;
i2c_driver i2c(defs::i2c::inst, defs::i2c::sda, defs::i2c::scl, defs::i2c::default_address, defs::i2c::default_speed, defs::i2c::default_timeout_us);
uart_driver uart(defs::uart::inst, defs::uart::rx, defs::uart::tx, defs::uart::baudrate);
gpio_driver gpio;
command_parser parser(gpio, i2c);

void led_blinking_task(void)
{
    static uint32_t blink_interval_ms = 500;
    static uint32_t start_ms = 0;
    static bool led_state = false;
    static bool initialized = false;

    if (!initialized) {
        gpio_init(defs::led);
        gpio_set_dir(defs::led, true);
        gpio_put(defs::led, 0);
        initialized = true;
    }

    // Blink every interval ms
    if ( board_millis() - start_ms < blink_interval_ms) {
        return; // not enough time
    }
    start_ms += blink_interval_ms;

    gpio_put(defs::led, led_state);
    led_state = !led_state;
}

int main() {
    stdio_init_all();

    tusb.init();
    gpio.init();

    while (true) {
        led_blinking_task();

        tusb.device_task();
        tusb.cdc_task();

        if (auto data = tusb.get_data()) {
            command cmd(*data);
            parser.parse_and_execute(cmd);
            tusb.rearm();
        }
    }
}
