#include "pico/stdlib.h"
#include <stdio.h>

#include "bsp/board_api.h"

#include "commands/commands.h"
#include "defs/defs.hpp"
#include "gpio/gpio_driver.h"
#include "i2c/i2c_driver.h"
#include "spi/spi_driver.h"
#include "tinyusb.hpp"
#include "uart/uart_driver.h"

using namespace drivers::gpio;
using namespace drivers::i2c;
using namespace drivers::spi;
using namespace drivers::uart;
using namespace lib::commands;

gpio_driver gpio;
i2c_driver i2c(defs::i2c::inst, defs::i2c::sda, defs::i2c::scl, defs::i2c::default_address, defs::i2c::default_speed, defs::i2c::default_timeout_us);
spi_driver spi(defs::spi::inst, defs::spi::rx, defs::spi::tx, defs::spi::sck, defs::spi::cs, defs::spi::default_speed);
tiny_usb tusb;
uart_driver uart(defs::uart::inst, defs::uart::rx, defs::uart::tx, defs::uart::baudrate);

command_parser parser(gpio, i2c, spi);

void led_blinking_task(void) {
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
  if (board_millis() - start_ms < blink_interval_ms) {
    return;  // not enough time
  }
  start_ms += blink_interval_ms;

  gpio_put(defs::led, led_state);
  led_state = !led_state;
}

int main() {
  stdio_init_all();

  gpio.init();
  i2c.init();
  spi.init();
  tusb.init();

  while (true) {
    led_blinking_task();

    tusb.device_task();
    tusb.cdc_task();

    auto data = tusb.get_data();
    if (!data) {
      continue;
    }

    command cmd(*data);
    auto response = parser.parse_and_execute(cmd);
    tusb.rearm();
    tusb.send_data(response);
  }
}
