#ifndef LIB_TINYUSB_HPP
#define LIB_TINYUSB_HPP

#include <span>
#include <optional>

#include "pico/stdlib.h"

class tiny_usb
{
private:
    bool ready = false;
    uint8_t command[64] = {};
    uint8_t size = 0;

public:
    tiny_usb();

    void init();
    void device_task();
    void cdc_task();

    std::optional<std::span<uint8_t>> get_data();
    void rearm();
};

#endif  // LIB_TINYUSB_HPP
