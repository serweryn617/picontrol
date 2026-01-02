#ifndef DRIVERS_SPI_SPI_DRIVER_H
#define DRIVERS_SPI_SPI_DRIVER_H

#include "hardware/spi.h"
#include "pico/stdlib.h"

namespace drivers::spi {

class spi_driver {
public:
  spi_driver(uint8_t spi_inst, uint8_t rx_pin, uint8_t tx_pin, uint8_t sck_pin, uint8_t cs_pin, uint32_t speed);

  void init();
  void set_speed(uint32_t speed);
  void cs_select();
  void cs_deselect();
  int read_data(uint8_t *buffer, uint32_t length = 1);
  int write_data(const uint8_t *buffer, uint32_t length = 1);

  void flash_read_status(uint8_t *buf);
  void flash_read(uint32_t addr, uint8_t *buf, size_t len);
  void flash_write_enable();
  bool flash_wait_done(uint32_t timeout_ms = 1000);
  bool flash_sector_erase(uint32_t addr);
  bool flash_chip_erase();
  bool flash_program(uint32_t addr, uint8_t *data, size_t len);

private:
  spi_inst_t *spi_inst_;
  uint8_t rx_pin_;
  uint8_t tx_pin_;
  uint8_t sck_pin_;
  uint8_t cs_pin_;
  uint32_t speed_;

  uint8_t _flash_read_status(uint8_t cmd);
};

}  // namespace drivers::spi

#endif  // DRIVERS_SPI_SPI_DRIVER_H
