from enum import IntEnum

VENDOR_ID = 0xcafe
PRODUCT_ID = 0x4001

class CommandType(IntEnum):
    NONE = 0
    GPIO_SET = 10
    GPIO_GET = 11
    UART = 20
    I2C_SET_SPEED = 30
    I2C_SET_ADDRESS = 31
    I2C_SET_TIMEOUT = 32
    I2C_READ = 33
    I2C_WRITE = 34
    SPI = 40
