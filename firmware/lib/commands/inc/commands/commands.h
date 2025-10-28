#ifndef LIB_COMMANDS_COMMANDS_H
#define LIB_COMMANDS_COMMANDS_H

#include "pico/stdlib.h"
#include "gpio/gpio_driver.h"
#include "i2c/i2c_driver.h"
#include <span>

namespace lib::commands {

enum class command_type : uint8_t {
    none = 0,
    gpio = 1,
    uart = 20,
    i2c_set_speed = 30,
    i2c_set_address = 31,
    i2c_set_timeout = 32,
    i2c_read = 33,
    i2c_write = 34,
    spi = 40,
};

struct command {
    command_type type;
    std::span<uint8_t> payload;

    command(std::span<uint8_t> data) {
        if (data.empty()) {
            type = command_type::none;
            payload = {};
            return;
        }

        type = static_cast<command_type>(data[0]);

        if (data.size() > 1) {
            payload = data.subspan(1);
        }
        else {
            payload = {};
        }
    }
};

class command_parser
{
public:
    command_parser(drivers::gpio::gpio_driver &_gpio, drivers::i2c::i2c_driver &_i2c);

    void parse_and_execute(command& cmd);
    void execute_gpio_command(std::span<uint8_t> payload);
    void execute_i2c_set_speed_command(std::span<uint8_t> payload);
    void execute_i2c_set_address_command(std::span<uint8_t> payload);
    void execute_i2c_set_timeout_command(std::span<uint8_t> payload);
    void execute_i2c_read_command(std::span<uint8_t> payload);
    void execute_i2c_write_command(std::span<uint8_t> payload);

private:
    drivers::gpio::gpio_driver &gpio;
    drivers::i2c::i2c_driver &i2c;

    uint8_t data_buffer[64 * 1024] = {};
};

}  // namespace lib::commands

#endif  // LIB_COMMANDS_COMMANDS_H
