#include "tinyusb.hpp"

#include <optional>
#include <span>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "bsp/board.h"
#include "tusb.h"

tiny_usb::tiny_usb() {}

void tiny_usb::init() {
  tusb_rhport_init_t dev_init = { .role = TUSB_ROLE_DEVICE, .speed = TUSB_SPEED_AUTO };
  tusb_init(BOARD_TUD_RHPORT, &dev_init);
}

void tiny_usb::device_task() {
  tud_task();
}

std::optional<std::span<uint8_t>> tiny_usb::get_data() {
  if (!ready) {
    return std::nullopt;
  }

  return std::span<uint8_t>(command, size);
}

void tiny_usb::rearm() {
  ready = false;
}

void tiny_usb::cdc_task() {
  if (!tud_cdc_available()) {
    return;
  }

  if (ready) {
    return;
  }

  size = tud_cdc_read(command, sizeof(command));
  ready = true;
}

void tiny_usb::send_data(std::span<uint8_t> data) {
  tud_cdc_write(data.data(), data.size());
  tud_cdc_write_flush();
}
