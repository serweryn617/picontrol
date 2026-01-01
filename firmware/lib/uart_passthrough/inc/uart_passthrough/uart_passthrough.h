#ifndef LIB_UART_PASSTHROUGH_H
#define LIB_UART_PASSTHROUGH_H

#include "tinyusb.hpp"
#include "uart/uart_driver.h"

namespace lib::uart_passthrough {

class uart_passthrough {
public:
  uart_passthrough(tiny_usb &tusb, drivers::uart::uart_driver &uart)
    : _tusb(tusb)
    , _uart(uart) {}

  void uart_passthrough_task();

private:
  tiny_usb &_tusb;
  drivers::uart::uart_driver &_uart;
};

}  // namespace lib::uart_passthrough

#endif  // LIB_UART_PASSTHROUGH_H
