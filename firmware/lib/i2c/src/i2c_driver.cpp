#include "i2c/i2c_driver.h"

namespace drivers::i2c {

i2c_driver::i2c_driver(uint8_t i2c_inst, uint8_t sda_pin, uint8_t scl_pin, uint8_t address, uint32_t speed)
    : i2c_inst_(i2c_inst == 0 ? i2c0 : i2c1)
    , sda_pin_(sda_pin)
    , scl_pin_(scl_pin)
    , address_(address)
    , speed_(speed)
{
}

void i2c_driver::init()
{
    i2c_init(i2c_inst_, speed_);
    gpio_set_function(sda_pin_, GPIO_FUNC_I2C);
    gpio_set_function(scl_pin_, GPIO_FUNC_I2C);
    gpio_pull_up(sda_pin_);
    gpio_pull_up(scl_pin_);
}

int i2c_driver::read_data(uint8_t *buffer, uint32_t length, uint32_t timeout_us)
{
    if (timeout_us) {
        return i2c_read_timeout_us(i2c_inst_, address_, buffer, length, false, timeout_us);
    }
    return i2c_read_blocking(i2c_inst_, address_, buffer, length, false);
}

int i2c_driver::write_data(const uint8_t *buffer, uint32_t length, uint32_t timeout_us)
{
    if (timeout_us) {
        return i2c_write_timeout_us(i2c_inst_, address_, buffer, length, false, timeout_us);
    }
    return i2c_write_blocking(i2c_inst_, address_, buffer, length, false);
}

}  // namespace drivers::i2c
