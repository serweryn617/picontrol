#include "commands/commands.h"

#include <cstring>

using namespace drivers::gpio;

namespace lib::commands {

void command_parser::execute_gpio_set_command(std::span<uint8_t> payload)
{
  if (payload.size() != 8) {
    set_status(command_status::parameter_error, 0);
    return;
  }

  uint32_t mask = word(payload, 0);
  uint32_t value = word(payload, 1);

  gpio.put_masked(mask, value);
  set_status(command_status::ok, 0);
}

void command_parser::execute_gpio_set_high_z_command(std::span<uint8_t> payload)
{
  if (payload.size() != 8) {
    set_status(command_status::parameter_error, 0);
    return;
  }

  uint32_t mask = word(payload, 0);
  uint32_t value = word(payload, 1);

  gpio.put_high_z_masked(mask, value);
  set_status(command_status::ok, 0);
}

void command_parser::execute_gpio_get_command()
{
  uint32_t gpio_state = gpio.get();
  memcpy(payload_buffer, &gpio_state, sizeof(gpio_state));
  set_status(command_status::ok, 4);
}

}  // namespace lib::commands
