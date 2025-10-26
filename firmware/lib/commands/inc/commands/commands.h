#ifndef LIB_COMMANDS_COMMANDS_H
#define LIB_COMMANDS_COMMANDS_H

#include "pico/stdlib.h"
#include "gpio/gpio_driver.h"
#include <span>

namespace lib::commands {

enum class command_type : uint8_t {
    none = 0,
    gpio = 1,
    uart = 2,
    i2c = 3,
    spi = 4,
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
    command_parser(drivers::gpio::gpio_driver &_gpio);

    void parse_and_execute(command& cmd);
    void execute_gpio_command(std::span<uint8_t> payload);

private:
    drivers::gpio::gpio_driver &gpio;
};

}  // namespace lib::commands

#endif  // LIB_COMMANDS_COMMANDS_H
