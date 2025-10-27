#include "tinyusb.hpp"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <span>
#include <optional>

#include "bsp/board.h"
#include "tusb.h"

tiny_usb::tiny_usb(tinyusb_context &_tusb_ctx)
    : tusb_ctx(_tusb_ctx)
{
}

void tiny_usb::init()
{
    tusb_rhport_init_t dev_init = {
        .role = TUSB_ROLE_DEVICE,
        .speed = TUSB_SPEED_AUTO
    };
    tusb_init(BOARD_TUD_RHPORT, &dev_init);

    tusb_ctx.ready = false;
}

void tiny_usb::device_task()
{
    tud_task();
}

std::optional<std::span<uint8_t>> tiny_usb::get_data()
{
    if (!tusb_ctx.ready) {
        return std::nullopt;
    }

    return std::span<uint8_t>(tusb_ctx.command, tusb_ctx.size);
}

void tiny_usb::rearm()
{
    tusb_ctx.ready = false;
}

void tiny_usb::cdc_task()
{
    if (!tud_cdc_available()) {
        return;
    }

    if (tusb_ctx.ready) {
        return;
    }

    uint8_t buffer[64];
    uint32_t bufsize = tud_cdc_read(buffer, sizeof(buffer));

    memcpy(tusb_ctx.command, buffer, bufsize);
    tusb_ctx.size = bufsize;
    tusb_ctx.ready = true;
}
