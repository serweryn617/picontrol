#include "gpio/gpio_driver.h"
#include "defs/defs.hpp"

namespace drivers::gpio {

void gpio_driver::init()
{
    for (auto &&pin : defs::gpio::gpios)
    {
        gpio_init(pin);
        gpio_set_dir(pin, true);
        gpio_put(pin, 0);
    }
}

void gpio_driver::set(uint8_t pin_num, bool value)
{
    gpio_put(defs::gpio::gpios[pin_num], value);
}

void gpio_driver::put_masked(uint32_t mask, uint32_t value)
{
    uint32_t pin_mask = 0;
    uint32_t val_mask = 0;

    for (uint32_t i = 0; i < defs::gpio::gpios.size(); i++)
    {
        bool pin_i_val = (value >> i) & 0b1;
        bool pin_i_mask = (mask >> i) & 0b1;

        pin_mask |= pin_i_mask << defs::gpio::gpios[i];
        val_mask |= pin_i_val << defs::gpio::gpios[i];
    }

    gpio_put_masked(pin_mask, val_mask);
}

}  // namespace drivers::i2c
