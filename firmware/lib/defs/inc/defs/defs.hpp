#ifndef LIB_DEFS_DEFS_HPP
#define LIB_DEFS_DEFS_HPP

#include <array>
#include <cstdint>

namespace defs {

namespace uart {
constexpr uint8_t inst = 1;
constexpr uint8_t tx = 4;
constexpr uint8_t rx = 5;
constexpr uint32_t baudrate = 9600;
}  // namespace uart

namespace i2c {
constexpr uint8_t inst = 1;
constexpr uint8_t sda = 6;
constexpr uint8_t scl = 7;
constexpr uint32_t default_speed = 400'000;
constexpr uint8_t default_address = 0x3c;
constexpr uint32_t default_timeout_us = 500'000;
}  // namespace i2c

namespace gpio {
constexpr std::array<uint8_t, 8> gpios = { 8, 9, 10, 11, 12, 13, 14, 15 };
}

namespace spi {
constexpr uint8_t inst = 0;
constexpr uint8_t rx = 16;
constexpr uint8_t cs = 17;
constexpr uint8_t sck = 18;
constexpr uint8_t tx = 19;
constexpr uint32_t default_speed = 50'000'000;  // 50MHz
}

constexpr uint8_t led = 25;

}  // namespace defs

#endif  // LIB_DEFS_DEFS_HPP
