#ifndef LIB_TINYUSB_HPP
#define LIB_TINYUSB_HPP

#include <span>
#include <optional>

#include "pico/stdlib.h"
#include "context.h"

class tiny_usb
{
private:
    tinyusb_context &tusb_ctx;
    bool ready();

public:
    tiny_usb(tinyusb_context &_tusb_ctx);

    void init();
    void device_task();
    void cdc_task();

    std::optional<std::span<uint8_t>> get_data();
    void rearm();
};

#endif  // LIB_TINYUSB_HPP
