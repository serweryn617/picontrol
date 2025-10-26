#ifndef LIB_DEFS_DEFS_HPP
#define LIB_DEFS_DEFS_HPP

#include <cstdint>

namespace defs {

namespace i2c {

constexpr uint8_t inst = 0;
constexpr uint8_t scl = 13;
constexpr uint8_t sda = 12;
constexpr uint32_t baudrate = 400'000;
constexpr uint8_t slave_address = 0x3c;

}  // namespace i2c

namespace uart {

constexpr uint8_t inst = 1;
constexpr uint8_t rx = 9;
constexpr uint8_t tx = 8;
constexpr uint32_t baudrate = 9600;

}  // namespace uart

}  // namespace defs

#endif  // LIB_DEFS_DEFS_HPP
