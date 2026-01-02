#ifndef LIB_COMMANDS_COMMANDS_H
#define LIB_COMMANDS_COMMANDS_H

#include "gpio/gpio_driver.h"
#include "i2c/i2c_driver.h"
#include "pico/stdlib.h"
#include "spi/spi_driver.h"
#include <optional>
#include <span>

namespace lib::commands {

enum class command_type : uint8_t {
  none = 0,
  enter_bootsel = 1,

  gpio_set = 10,
  gpio_set_high_z = 11,
  gpio_get = 12,

  uart = 20,

  i2c_set_speed = 30,
  i2c_set_address = 31,
  i2c_set_timeout = 32,
  i2c_read = 33,
  i2c_write = 34,

  spi_set_speed = 40,
  spi_cs_select = 41,
  spi_cs_deselect = 42,
  spi_read = 43,
  spi_write = 44,

  flash_read_status = 50,
  flash_read = 51,
  flash_sector_erase = 52,
  flash_chip_erase = 53,
  flash_program = 54,
};

enum class command_status : uint8_t {
  ok = 0,
  generic_error = 1,
  parameter_error = 2,
  i2c_error = 3,
  flash_busy_timeout = 4,
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
    } else {
      payload = {};
    }
  }
};

class command_parser {
public:
  command_parser(drivers::gpio::gpio_driver &_gpio, drivers::i2c::i2c_driver &_i2c, drivers::spi::spi_driver &_spi);

  std::span<uint8_t> parse_and_execute(command &cmd);
  void set_status(command_status status, uint32_t payload_length);

  void execute_enter_bootsel_command();

  void execute_gpio_set_command(std::span<uint8_t> payload);
  void execute_gpio_set_high_z_command(std::span<uint8_t> payload);
  void execute_gpio_get_command();

  void execute_i2c_set_speed_command(std::span<uint8_t> payload);
  void execute_i2c_set_address_command(std::span<uint8_t> payload);
  void execute_i2c_set_timeout_command(std::span<uint8_t> payload);
  void execute_i2c_read_command(std::span<uint8_t> payload);
  void execute_i2c_write_command(std::span<uint8_t> payload);

  void execute_spi_set_speed_command(std::span<uint8_t> payload);
  void execute_spi_cs_select_command();
  void execute_spi_cs_deselect_command();
  void execute_spi_read_command(std::span<uint8_t> payload);
  void execute_spi_write_command(std::span<uint8_t> payload);

  void execute_flash_read_status();
  void execute_flash_read(std::span<uint8_t> payload);
  void execute_flash_sector_erase(std::span<uint8_t> payload);
  void execute_flash_chip_erase();
  void execute_flash_program(std::span<uint8_t> payload);

private:
  drivers::gpio::gpio_driver &gpio;
  drivers::i2c::i2c_driver &i2c;
  drivers::spi::spi_driver &spi;

  uint8_t data_buffer[9 * 1024] = {};
  uint8_t *payload_buffer = &data_buffer[1];
  uint32_t data_length = 0;

  uint32_t word(std::span<uint8_t> data, size_t word_num = 0);
};

}  // namespace lib::commands

#endif  // LIB_COMMANDS_COMMANDS_H
