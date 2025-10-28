#include "commands/commands.h"

#include <cstring>

using namespace drivers::gpio;
using namespace drivers::i2c;

static constexpr uint32_t word(std::span<uint8_t> data, size_t word_num = 0) {
  word_num *= 4;
  return static_cast<uint32_t>(data[word_num]) |
    (static_cast<uint32_t>(data[word_num + 1]) << 8) |
    (static_cast<uint32_t>(data[word_num + 2]) << 16) |
    (static_cast<uint32_t>(data[word_num + 3]) << 24);
}

namespace lib::commands {

command_parser::command_parser(gpio_driver &_gpio, i2c_driver &_i2c)
  : gpio(_gpio)
  , i2c(_i2c)
{
}

std::optional<std::span<uint8_t>> command_parser::parse_and_execute(command& cmd)
{
  switch (cmd.type) {
  case command_type::gpio_set:
    execute_gpio_set_command(cmd.payload);
    break;

  case command_type::gpio_get:
    execute_gpio_get_command();
    return std::span<uint8_t>(data_buffer, 4);

  case command_type::i2c_set_speed:
    execute_i2c_set_speed_command(cmd.payload);
    break;

  case command_type::i2c_set_address:
    execute_i2c_set_address_command(cmd.payload);
    break;

  case command_type::i2c_set_timeout:
    execute_i2c_set_timeout_command(cmd.payload);
    break;

  case command_type::i2c_read:
    execute_i2c_read_command(cmd.payload);
    break;

  case command_type::i2c_write:
    execute_i2c_write_command(cmd.payload);
    break;

  default:
    break;
  }

  return std::nullopt;
}

void command_parser::execute_gpio_set_command(std::span<uint8_t> payload)
{
  if (payload.size() != 8) {
    return;
  }

  uint32_t mask = word(payload, 0);
  uint32_t value = word(payload, 1);

  gpio.put_masked(mask, value);
}

void command_parser::execute_gpio_get_command()
{
  uint32_t gpio_state = gpio.get();
  memcpy(data_buffer, &gpio_state, sizeof(gpio_state));
}

void command_parser::execute_i2c_set_speed_command(std::span<uint8_t> payload)
{
  if (payload.size() != 4) {
    return;
  }

  uint32_t speed = word(payload, 0);

  i2c.set_speed(speed);
}

void command_parser::execute_i2c_set_address_command(std::span<uint8_t> payload)
{
  if (payload.size() != 4) {
    return;
  }

  uint32_t address = word(payload, 0);

  i2c.set_address(address);
}

void command_parser::execute_i2c_set_timeout_command(std::span<uint8_t> payload)
{
  if (payload.size() != 4) {
    return;
  }

  uint32_t timeout = word(payload, 0);

  i2c.set_address(timeout);
}

void command_parser::execute_i2c_read_command(std::span<uint8_t> payload)
{
  if (payload.size() != 4) {
    return;
  }

  uint32_t length = word(payload, 0);

  i2c.read_data(data_buffer, length);
}

void command_parser::execute_i2c_write_command(std::span<uint8_t> payload)
{
  if (payload.size() == 0) {
    return;
  }

  i2c.write_data(payload.data(), payload.size());
}

}  // namespace lib::commands
