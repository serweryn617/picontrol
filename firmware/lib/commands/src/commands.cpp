#include "commands/commands.h"

using namespace drivers::gpio;

namespace lib::commands {

command_parser::command_parser(drivers::gpio::gpio_driver &_gpio)
  : gpio(_gpio)
{
}

void command_parser::parse_and_execute(command& cmd)
{
  if (cmd.type == command_type::gpio) {
    execute_gpio_command(cmd.payload);
  }
}

void command_parser::execute_gpio_command(std::span<uint8_t> payload)
{
  if (payload.size() != 8) {
    return;
  }

  uint32_t mask =
    static_cast<uint32_t>(payload[0]) |
    (static_cast<uint32_t>(payload[1]) << 8) |
    (static_cast<uint32_t>(payload[2]) << 16) |
    (static_cast<uint32_t>(payload[3]) << 24);

  uint32_t value =
    static_cast<uint32_t>(payload[4]) |
    (static_cast<uint32_t>(payload[5]) << 8) |
    (static_cast<uint32_t>(payload[6]) << 16) |
    (static_cast<uint32_t>(payload[7]) << 24);

  gpio.put_masked(mask, value);
}

}  // namespace lib::commands
