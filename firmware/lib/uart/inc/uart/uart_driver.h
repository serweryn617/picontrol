#ifndef DRIVERS_UART_UART_DRIVER_H
#define DRIVERS_UART_UART_DRIVER_H

#include "hardware/uart.h"
#include "pico/stdlib.h"

namespace drivers::uart {

class uart_driver {
public:
  uart_driver(uint8_t uart_inst, uint8_t rx, uint8_t tx, uint32_t baudrate);

  void init();
  bool read_line(uint8_t *buffer, uint32_t max_length, uint32_t timeout_ms = 100);

private:
  uart_inst_t *uart_inst_;
  uint8_t rx_;
  uint8_t tx_;
  uint32_t baudrate_;
};

}  // namespace drivers::uart

#endif  // DRIVERS_UART_UART_DRIVER_H
