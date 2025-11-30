#include "uart/uart_driver.h"
#include "pico/time.h"

namespace drivers::uart {

uart_driver::uart_driver(uint8_t uart_inst, uint8_t rx, uint8_t tx, uint32_t baudrate)
  : uart_inst_(uart_inst == 0 ? uart0 : uart1)
  , rx_(rx)
  , tx_(tx)
  , baudrate_(baudrate) {}

void uart_driver::init() {
  gpio_set_function(rx_, GPIO_FUNC_UART);
  gpio_set_function(tx_, GPIO_FUNC_UART);
  uart_init(uart_inst_, baudrate_);
}

bool uart_driver::read_line(uint8_t *buffer, uint32_t max_length, uint32_t timeout_ms) {
  size_t idx = 0;
  absolute_time_t deadline = make_timeout_time_ms(timeout_ms);

  while (absolute_time_diff_us(get_absolute_time(), deadline) > 0) {
    if (uart_is_readable(uart_inst_)) {
      char ch = uart_getc(uart_inst_);
      if (ch == '\r')
        continue;
      if (ch == '\n') {
        buffer[idx] = '\0';
        return true;
      }
      if (idx < max_length - 1) {
        buffer[idx++] = ch;
      }
    }
  }

  buffer[idx] = '\0';
  return false;
}

bool uart_driver::is_readable() {
  return uart_is_readable(uart_inst_);
}

char uart_driver::getc() {
  return uart_getc(uart_inst_);
}

}  // namespace drivers::uart
