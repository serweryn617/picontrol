#include "i2c/i2c_driver.h"

namespace drivers::i2c {

i2c_driver::i2c_driver(uint8_t i2c_inst, uint8_t sda_pin, uint8_t scl_pin, uint8_t address, uint32_t speed, uint32_t timeout_us)
  : i2c_inst_(i2c_inst == 0 ? i2c0 : i2c1)
  , sda_pin_(sda_pin)
  , scl_pin_(scl_pin)
  , address_(address)
  , speed_(speed)
  , timeout_us_(timeout_us) {}

void i2c_driver::init() {
  i2c_init(i2c_inst_, speed_);
  gpio_set_function(sda_pin_, GPIO_FUNC_I2C);
  gpio_set_function(scl_pin_, GPIO_FUNC_I2C);
  gpio_pull_up(sda_pin_);
  gpio_pull_up(scl_pin_);
}

void i2c_driver::set_speed(uint32_t speed) {
  i2c_set_baudrate(i2c_inst_, speed);
}

void i2c_driver::set_address(uint8_t address) {
  address_ = address;
}

void i2c_driver::set_timeout(uint32_t timeout_us) {
  timeout_us_ = timeout_us;
}

int i2c_driver::read_data(uint8_t *buffer, uint32_t length) {
  return i2c_read_timeout_us(i2c_inst_, address_, buffer, length, false, timeout_us_);
}

int i2c_driver::write_data(const uint8_t *buffer, uint32_t length) {
  return i2c_write_timeout_us(i2c_inst_, address_, buffer, length, false, timeout_us_);
}

}  // namespace drivers::i2c
