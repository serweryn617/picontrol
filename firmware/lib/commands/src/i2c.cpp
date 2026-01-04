#include "commands/commands.h"
#include <cstring>

using namespace drivers::spi;

namespace lib::commands {

void command_parser::execute_i2c_set_speed_command(std::span<uint8_t> payload) {
  if (payload.size() != 4) {
    set_status(command_status::parameter_error, 0);
    return;
  }

  uint32_t speed = word(payload, 0);

  i2c.set_speed(speed);
  set_status(command_status::ok, 0);
}

void command_parser::execute_i2c_set_address_command(std::span<uint8_t> payload) {
  if (payload.size() != 1) {
    set_status(command_status::parameter_error, 0);
    return;
  }

  i2c.set_address(payload[0]);
  set_status(command_status::ok, 0);
}

void command_parser::execute_i2c_set_timeout_command(std::span<uint8_t> payload) {
  if (payload.size() != 4) {
    set_status(command_status::parameter_error, 0);
    return;
  }

  uint32_t timeout = word(payload, 0);

  i2c.set_timeout(timeout);
  set_status(command_status::ok, 0);
}

void command_parser::execute_i2c_read_command(std::span<uint8_t> payload) {
  if (payload.size() != 4) {
    set_status(command_status::parameter_error, 0);
    return;
  }

  uint32_t length = word(payload, 0);

  int result = i2c.read_data(payload_buffer, length);
  if (result < 0) {
    set_status(command_status::i2c_error, 0);
    return;
  }
  set_status(command_status::ok, result);
}

void command_parser::execute_i2c_write_command(std::span<uint8_t> payload) {
  if (payload.size() == 0) {
    set_status(command_status::parameter_error, 0);
    return;
  }

  int result = i2c.write_data(payload.data(), payload.size());
  if (result < 0) {
    set_status(command_status::i2c_error, 0);
    return;
  }
  set_status(command_status::ok, 0);
}

}  // namespace lib::commands
