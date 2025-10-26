#ifndef LIB_GPIO_GPIO_DRIVER_H
#define LIB_GPIO_GPIO_DRIVER_H

#include "pico/stdlib.h"

namespace drivers::gpio {

class gpio_driver
{
public:
    void init();
    void set(uint8_t pin_num, bool value);
    void put_masked(uint32_t value);
};

}  // namespace drivers::gpio

#endif  // LIB_GPIO_GPIO_DRIVER_H
