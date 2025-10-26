#ifndef LIB_DEFS_DEFS_HPP
#define LIB_DEFS_DEFS_HPP

#include <cstdint>
#include <array>

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
constexpr uint32_t baudrate = 400'000;
constexpr uint8_t slave_address = 0x3c;
}  // namespace i2c

namespace gpio {
constexpr std::array<uint8_t, 8> gpios = { 8, 9, 10, 11, 12, 13, 14, 15 };
}

constexpr uint8_t led = 25;

}  // namespace defs

#endif  // LIB_DEFS_DEFS_HPP
