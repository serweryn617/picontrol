#include "gpio/gpio_driver.h"
#include "defs/defs.hpp"

namespace drivers::gpio {

void gpio_driver::init() {
  for (auto &&pin : defs::gpio::gpios) {
    gpio_init(pin);
    gpio_set_dir(pin, true);
    gpio_put(pin, 0);
  }
}

void gpio_driver::put_masked(uint32_t mask, uint32_t value) {
  uint32_t pin_mask = 0;
  uint32_t val_mask = 0;

  for (uint32_t i = 0; i < defs::gpio::gpios.size(); i++) {
    bool pin_i_val = (value >> i) & 0b1;
    bool pin_i_mask = (mask >> i) & 0b1;

    pin_mask |= pin_i_mask << defs::gpio::gpios[i];
    val_mask |= pin_i_val << defs::gpio::gpios[i];
  }

  gpio_put_masked(pin_mask, val_mask);
  gpio_set_dir_masked(pin_mask, 0xFFFFFFFF);
}

void gpio_driver::put_high_z_masked(uint32_t mask, uint32_t value) {
  uint32_t pin_mask = 0;
  uint32_t dir_mask = 0;  // 0 - input, 1 - output

  for (uint32_t i = 0; i < defs::gpio::gpios.size(); i++) {
    bool pin_i_mask = (mask >> i) & 0b1;
    bool pin_i_dir = !((value >> i) & 0b1);

    pin_mask |= pin_i_mask << defs::gpio::gpios[i];
    dir_mask |= pin_i_dir << defs::gpio::gpios[i];
  }

  gpio_put_masked(pin_mask, 0);
  gpio_set_dir_masked(pin_mask, dir_mask);
}

uint32_t gpio_driver::get() {
  uint32_t gpio_state = gpio_get_all();
  uint32_t out_value = 0;

  for (uint32_t i = 0; i < defs::gpio::gpios.size(); i++) {
    bool pin_i_val = (gpio_state >> defs::gpio::gpios[i]) & 0b1;
    out_value |= pin_i_val << i;
  }

  return out_value;
}

}  // namespace drivers::i2c
