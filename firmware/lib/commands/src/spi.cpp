#include "commands/commands.h"
#include <cstring>

using namespace drivers::spi;

namespace lib::commands {

void command_parser::execute_spi_set_speed_command(std::span<uint8_t> payload) {
  if (payload.size() != 4) {
    set_status(command_status::parameter_error, 0);
    return;
  }

  uint32_t value = word(payload, 0);

  spi.set_speed(value);
  set_status(command_status::ok, 0);
}

void command_parser::execute_spi_cs_select_command() {
  spi.cs_select();
  set_status(command_status::ok, 0);
}

void command_parser::execute_spi_cs_deselect_command() {
  spi.cs_deselect();
  set_status(command_status::ok, 0);
}

void command_parser::execute_spi_read_command(std::span<uint8_t> payload) {
  if (payload.size() != 4) {
    set_status(command_status::parameter_error, 0);
    return;
  }

  uint32_t length = word(payload, 0);
  int result = spi.read_data(payload_buffer, length);

  set_status(command_status::ok, result);
}

void command_parser::execute_spi_write_command(std::span<uint8_t> payload) {
  if (payload.size() == 0) {
    set_status(command_status::parameter_error, 0);
    return;
  }

  spi.write_data(payload.data(), payload.size());
  set_status(command_status::ok, 0);
}

void command_parser::execute_flash_read(std::span<uint8_t> payload) {
  if (payload.size() != 8) {
    set_status(command_status::parameter_error, 0);
    return;
  }

  uint32_t address = word(payload, 0);
  uint32_t length = word(payload, 1);

  spi.flash_read(address, payload_buffer, length);
  set_status(command_status::ok, length);
}

void command_parser::execute_flash_sector_erase(std::span<uint8_t> payload) {
  if (payload.size() != 4) {
    set_status(command_status::parameter_error, 0);
    return;
  }

  uint32_t address = word(payload, 0);
  spi.flash_sector_erase(address);

  set_status(command_status::ok, 0);
}

void command_parser::execute_flash_page_program(std::span<uint8_t> payload) {
  if (payload.size() != 4 + 256) {
    set_status(command_status::parameter_error, 0);
    return;
  }

  uint32_t address = word(payload, 0);

  spi.flash_page_program(address, &payload[4]);
  set_status(command_status::ok, 0);
}

}  // namespace lib::commands
