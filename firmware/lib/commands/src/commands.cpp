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
  if (payload.size() != 4) {
    return;
  }
}

}  // namespace lib::commands
