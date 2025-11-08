#ifndef LIB_GPIO_GPIO_DRIVER_H
#define LIB_GPIO_GPIO_DRIVER_H

#include "pico/stdlib.h"

namespace drivers::gpio {

class gpio_driver {
public:
  void init();
  void put_masked(uint32_t mask, uint32_t value);
  void put_high_z_masked(uint32_t mask, uint32_t value);
  uint32_t get();
};

}  // namespace drivers::gpio

#endif  // LIB_GPIO_GPIO_DRIVER_H
