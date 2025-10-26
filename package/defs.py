from enum import IntEnum

VENDOR_ID = 0xcafe
PRODUCT_ID = 0x4015

class CommandType(IntEnum):
    NONE = 0x00
    GPIO = 0x01
    UART = 0x02
    I2C  = 0x03
    SPI  = 0x04
