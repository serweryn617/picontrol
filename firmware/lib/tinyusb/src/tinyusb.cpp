#include "tinyusb.hpp"

#include <cstring>
#include <optional>
#include <span>
#include <stdio.h>
#include <stdlib.h>

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
  if (!_command_received) {
    return std::nullopt;
  }

  return std::span<uint8_t>(&_transaction_buffer[8], _size);
}

void tiny_usb::rearm() {
  _buffer_pointer -= (8 + _size);
  std::memmove(_transaction_buffer, &_transaction_buffer[8 + _size], _buffer_pointer);

  _command_received = false;
}

// void tiny_usb::cdc_task() {
//   if (tud_cdc_available() < 8) {  // TODO: always read, just discard if too small
//     return;
//   }

//   if (_command_received) {
//     return;
//   }

//   uint32_t command_magic_received = 0;
//   uint32_t payload_length_received = 0;
//   tud_cdc_read(&command_magic_received, sizeof(command_magic_received));
//   tud_cdc_read(&payload_length_received, sizeof(payload_length_received));

//   bool invalid_magic = command_magic_received != 0xBADCAB1E;
//   bool payload_too_small = payload_length_received < 1;
//   bool payload_too_big = payload_length_received > sizeof(_transaction_buffer);

//   if (invalid_magic || payload_too_small || payload_too_big) {
//     return;
//   }

//   uint32_t size_to_read = payload_length_received;
//   uint32_t read_buffer_pointer = 0;
//   for (uint32_t i = 0; i < 100; i++) {
//     if (!tud_cdc_available()) {
//       sleep_ms(1);
//       continue;
//     }

//     uint32_t bytes_read = tud_cdc_read(&_transaction_buffer[read_buffer_pointer], size_to_read);
//     size_to_read -= bytes_read;
//     read_buffer_pointer += bytes_read;

//     if (size_to_read == 0) {
//       _size = payload_length_received;
//       _command_received = true;
//       break;
//     }
//   }

//   // TODO: timeout
// }

void tiny_usb::send_data(std::span<uint8_t> data) {
  tud_cdc_write(data.data(), data.size());
  tud_cdc_write_flush();
}

void tiny_usb::cdc_task() {
  if (!tud_cdc_available()) {
    return;
  }

  if (_command_received) {
    return;
  }

  uint32_t size_to_read = sizeof(_transaction_buffer) - _buffer_pointer;
  uint32_t bytes_read = tud_cdc_read(&_transaction_buffer[_buffer_pointer], size_to_read);
  _buffer_pointer += bytes_read;

  if (_state == State::ReadingTransactionHeader) {
    search_for_transaction_header();
  }

  if (_state == State::TransactionHeaderFound) {
    get_payload_size();
  }

  if (_state == State::ReadingCommand) {
    get_command();
  }
}

void tiny_usb::search_for_transaction_header() {
  uint32_t transacion_magic = 0xBADCAB1E;
  uint32_t idx = 0;

  for (; idx < _buffer_pointer - sizeof(transacion_magic); idx++) {
    if (std::memcmp(&_transaction_buffer[idx], &transacion_magic, sizeof(transacion_magic)) == 0) {
      _state = State::TransactionHeaderFound;
      break;
    }
  }

  _buffer_pointer -= idx;
  std::memmove(_transaction_buffer, &_transaction_buffer[idx], _buffer_pointer);
}

void tiny_usb::get_payload_size() {
  if (_buffer_pointer < 8) {
    return;
  }

  _size = static_cast<uint32_t>(_transaction_buffer[4]) | (static_cast<uint32_t>(_transaction_buffer[5]) << 8) |
          (static_cast<uint32_t>(_transaction_buffer[6]) << 16) | (static_cast<uint32_t>(_transaction_buffer[7]) << 24);
  _state = State::ReadingCommand;
}

void tiny_usb::get_command() {
  if (_buffer_pointer < 8 + _size) {
    return;
  }

  _command_received = true;
  _state = State::ReadingTransactionHeader;
}
