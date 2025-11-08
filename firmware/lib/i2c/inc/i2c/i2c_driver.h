#ifndef DRIVERS_I2C_I2C_DRIVER_H
#define DRIVERS_I2C_I2C_DRIVER_H

#include "hardware/i2c.h"
#include "pico/stdlib.h"

namespace drivers::i2c {

class i2c_driver {
public:
  i2c_driver(uint8_t i2c_inst, uint8_t sda_pin, uint8_t scl_pin, uint8_t address, uint32_t speed, uint32_t timeout_us);

  void init();
  void set_speed(uint32_t speed);
  void set_address(uint8_t address);
  void set_timeout(uint32_t timeout_us);
  int read_data(uint8_t *buffer, uint32_t length = 1);
  int write_data(const uint8_t *buffer, uint32_t length = 1);

private:
  i2c_inst_t *i2c_inst_;
  uint8_t sda_pin_;
  uint8_t scl_pin_;
  uint8_t address_;
  uint32_t speed_;
  uint32_t timeout_us_;
};

}  // namespace drivers::i2c

#endif  // DRIVERS_I2C_I2C_DRIVER_H
