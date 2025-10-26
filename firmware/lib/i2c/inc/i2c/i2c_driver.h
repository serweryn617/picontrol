#ifndef DRIVERS_I2C_I2C_DRIVER_H
#define DRIVERS_I2C_I2C_DRIVER_H

#include "pico/stdlib.h"
#include "hardware/i2c.h"

namespace drivers::i2c {

class i2c_driver
{
public:
    i2c_driver(uint8_t i2c_inst, uint8_t sda_pin, uint8_t scl_pin, uint8_t address, uint32_t speed);

    void init();
    int read_data(uint8_t *buffer, uint32_t length = 1, uint32_t timeout_us = 0);
    int write_data(const uint8_t *buffer, uint32_t length = 1, uint32_t timeout_us = 0);

private:
    i2c_inst_t *i2c_inst_;
    uint8_t sda_pin_;
    uint8_t scl_pin_;
    uint8_t address_;
    uint32_t speed_;
};

}  // namespace drivers::i2c

#endif  // DRIVERS_I2C_I2C_DRIVER_H
