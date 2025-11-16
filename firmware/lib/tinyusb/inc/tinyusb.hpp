#ifndef LIB_TINYUSB_HPP
#define LIB_TINYUSB_HPP

#include <optional>
#include <span>

#include "pico/stdlib.h"

enum class State : uint8_t { ReadingTransactionHeader, TransactionHeaderFound, ReadingCommand };

// TODO: Add command reading timeout

class tiny_usb {
private:
  uint8_t _transaction_buffer[9 * 1024] = {};
  uint32_t _size = 0;
  uint32_t _buffer_pointer = 0;

  bool _command_received = false;

  State _state = State::ReadingTransactionHeader;

  void search_for_transaction_header();
  void get_payload_size();
  void get_command();

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
