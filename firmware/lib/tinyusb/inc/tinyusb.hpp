#ifndef LIB_TINYUSB_HPP
#define LIB_TINYUSB_HPP

#include <optional>
#include <span>

#include "pico/stdlib.h"

class tiny_usb {
private:
  bool ready = false;
  uint8_t command[1024] = {};
  uint8_t size = 0;

public:
  tiny_usb();

  void init();
  void device_task();
  void cdc_task();

  std::optional<std::span<uint8_t>> get_data();
  void send_data(std::span<uint8_t> data);
  void rearm();
};

#endif  // LIB_TINYUSB_HPP
