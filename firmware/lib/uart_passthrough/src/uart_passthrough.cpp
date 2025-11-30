#include "uart_passthrough/uart_passthrough.h"

namespace lib::uart_passthrough {

void uart_passthrough::uart_passthrough_task() {
  int processed = 0;

  while (_uart.is_readable() && _tusb.uart_write_available() && processed < 64) {
    uint8_t ch = _uart.getc();
    _tusb.uart_write_char(ch);  // CDC interface #1
    processed++;
  }

  _tusb.uart_write_flush();
}

}  // namespace lib::uart
