#include "commands/commands.h"
#include "defs/defs.hpp"
#include "pico/bootrom.h"

#include <cstdio>
#include <cstring>

using namespace drivers::gpio;
using namespace drivers::i2c;
using namespace drivers::spi;

namespace lib::commands {

command_parser::command_parser(gpio_driver &_gpio, i2c_driver &_i2c, spi_driver &_spi)
  : gpio(_gpio)
  , i2c(_i2c)
  , spi(_spi) {}

uint32_t command_parser::word(std::span<uint8_t> data, size_t word_num) {
  word_num *= 4;
  return static_cast<uint32_t>(data[word_num]) | (static_cast<uint32_t>(data[word_num + 1]) << 8) | (static_cast<uint32_t>(data[word_num + 2]) << 16) |
         (static_cast<uint32_t>(data[word_num + 3]) << 24);
}

std::span<uint8_t> command_parser::parse_and_execute(command &cmd) {
  set_status(command_status::generic_error, 0);

  switch (cmd.type) {
    case command_type::enter_bootsel:
      execute_enter_bootsel_command();
      break;

    case command_type::gpio_set:
      execute_gpio_set_command(cmd.payload);
      break;

    case command_type::gpio_set_high_z:
      execute_gpio_set_high_z_command(cmd.payload);
      break;

    case command_type::gpio_get:
      execute_gpio_get_command();
      break;

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

    case command_type::spi_set_speed:
      execute_spi_set_speed_command(cmd.payload);
      break;

    case command_type::spi_cs_select:
      execute_spi_cs_select_command();
      break;

    case command_type::spi_cs_deselect:
      execute_spi_cs_deselect_command();
      break;

    case command_type::spi_read:
      execute_spi_read_command(cmd.payload);
      break;

    case command_type::spi_write:
      execute_spi_write_command(cmd.payload);
      break;

    case command_type::flash_read_status:
      execute_flash_read_status();
      break;

    case command_type::flash_read:
      execute_flash_read(cmd.payload);
      break;

    case command_type::flash_sector_erase:
      execute_flash_sector_erase(cmd.payload);
      break;

    case command_type::flash_chip_erase:
      execute_flash_chip_erase();
      break;

    case command_type::flash_program:
      execute_flash_program(cmd.payload);
      break;

    default:
      break;
  }

  return std::span<uint8_t>(data_buffer, data_length);
}

void command_parser::set_status(command_status status, uint32_t payload_length) {
  data_buffer[0] = static_cast<uint8_t>(status);
  data_length = 1 + payload_length;
}

void command_parser::execute_enter_bootsel_command() {
  printf("execute_enter_bootsel_command\n");

  uint32_t usb_activity_gpio_pin_mask = 1 << defs::led;
  uint32_t disable_interface_mask = 0;
  rom_reset_usb_boot(usb_activity_gpio_pin_mask, disable_interface_mask);
}

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
