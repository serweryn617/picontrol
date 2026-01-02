#include "spi/spi_driver.h"

namespace drivers::spi {

static constexpr uint32_t flash_page_size = 256;
static constexpr uint32_t flash_sector_size = 4096;

static constexpr uint8_t flash_cmd_page_program = 0x02;
static constexpr uint8_t flash_cmd_read = 0x03;
static constexpr uint8_t flash_cmd_write_en = 0x06;
static constexpr uint8_t flash_cmd_sector_erase = 0x20;
static constexpr uint8_t flash_cmd_chip_erase = 0xC7;

static constexpr uint8_t flash_status_busy_mask = 0x01;

static constexpr uint8_t flash_cmd_read_status_register_1 = 0x05;
static constexpr uint8_t flash_cmd_read_status_register_2 = 0x35;
static constexpr uint8_t flash_cmd_read_status_register_3 = 0x15;

uint8_t spi_driver::_flash_read_status(uint8_t cmd) {
  uint8_t status = 0;

  cs_select();
  spi_write_blocking(spi_inst_, &cmd, 1);
  spi_read_blocking(spi_inst_, 0, &status, 1);
  cs_deselect();

  return status;
}

void spi_driver::flash_read_status(uint8_t *buf) {
  buf[0] = _flash_read_status(flash_cmd_read_status_register_1);
  buf[1] = _flash_read_status(flash_cmd_read_status_register_2);
  buf[2] = _flash_read_status(flash_cmd_read_status_register_3);
}

void spi_driver::flash_read(uint32_t addr, uint8_t *buf, size_t len) {
  cs_select();
  uint8_t cmdbuf[4] = { flash_cmd_read, static_cast<uint8_t>(addr >> 16), static_cast<uint8_t>(addr >> 8), static_cast<uint8_t>(addr) };
  spi_write_blocking(spi_inst_, cmdbuf, 4);
  spi_read_blocking(spi_inst_, 0, buf, len);
  cs_deselect();
}

void spi_driver::flash_write_enable() {
  cs_select();
  uint8_t cmd = flash_cmd_write_en;
  spi_write_blocking(spi_inst_, &cmd, 1);
  cs_deselect();
}

bool spi_driver::flash_wait_done(uint32_t timeout_ms) {
  uint8_t status;

  for (uint32_t i = 0; i < timeout_ms; i++) {
    cs_select();
    uint8_t buf[2] = { flash_cmd_read_status_register_1, 0 };
    spi_write_read_blocking(spi_inst_, buf, buf, 2);
    cs_deselect();
    status = buf[1];

    if ((status & flash_status_busy_mask) == 0) {
      return true;
    }

    sleep_ms(1);
  }

  return false;
}

bool spi_driver::flash_sector_erase(uint32_t addr) {
  uint8_t cmdbuf[4] = { flash_cmd_sector_erase, static_cast<uint8_t>(addr >> 16), static_cast<uint8_t>(addr >> 8), static_cast<uint8_t>(addr) };
  flash_write_enable();
  cs_select();
  spi_write_blocking(spi_inst_, cmdbuf, 4);
  cs_deselect();
  return flash_wait_done();
}

bool spi_driver::flash_chip_erase() {
  uint8_t cmd = flash_cmd_chip_erase;
  flash_write_enable();
  cs_select();
  spi_write_blocking(spi_inst_, &cmd, 1);
  cs_deselect();
  // TODO: Use user param
  return flash_wait_done(60'000);
}

bool spi_driver::flash_program(uint32_t addr, uint8_t *data, size_t len) {
  uint8_t cmdbuf[4] = { flash_cmd_page_program, static_cast<uint8_t>(addr >> 16), static_cast<uint8_t>(addr >> 8), static_cast<uint8_t>(addr) };
  flash_write_enable();
  cs_select();
  spi_write_blocking(spi_inst_, cmdbuf, 4);
  spi_write_blocking(spi_inst_, data, len);
  cs_deselect();
  return flash_wait_done();
}

}  // namespace drivers::spi
