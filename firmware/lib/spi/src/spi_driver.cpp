#include "spi/spi_driver.h"

namespace drivers::spi {

spi_driver::spi_driver(uint8_t spi_inst, uint8_t rx_pin, uint8_t tx_pin, uint8_t sck_pin, uint8_t cs_pin, uint32_t speed)
  : spi_inst_(spi_inst == 0 ? spi0 : spi1)
  , rx_pin_(rx_pin)
  , tx_pin_(tx_pin)
  , sck_pin_(sck_pin)
  , cs_pin_(cs_pin)
  , speed_(speed) {}

void spi_driver::init() {
  spi_init(spi_inst_, speed_);
  gpio_set_function(rx_pin_, GPIO_FUNC_SPI);
  gpio_set_function(tx_pin_, GPIO_FUNC_SPI);
  gpio_set_function(sck_pin_, GPIO_FUNC_SPI);

  gpio_init(cs_pin_);
  gpio_put(cs_pin_, 1);
  gpio_set_dir(cs_pin_, GPIO_OUT);
}

void spi_driver::cs_select() {
  asm volatile("nop \n nop \n nop");  // FIXME
  gpio_put(cs_pin_, 0);
  asm volatile("nop \n nop \n nop");  // FIXME
}

void spi_driver::cs_deselect() {
  asm volatile("nop \n nop \n nop");  // FIXME
  gpio_put(cs_pin_, 1);
  asm volatile("nop \n nop \n nop");  // FIXME
}

void spi_driver::set_speed(uint32_t speed) {
  spi_set_baudrate(spi_inst_, speed);
}

int spi_driver::read_data(uint8_t *buffer, uint32_t length) {
  return spi_read_blocking(spi_inst_, 0, buffer, length);
}

int spi_driver::write_data(const uint8_t *buffer, uint32_t length) {
  return spi_write_blocking(spi_inst_, buffer, length);
}

}  // namespace drivers::spi
