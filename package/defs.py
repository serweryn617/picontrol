from enum import IntEnum

VENDOR_ID = 0xCAFE
PRODUCT_ID = 0x4001


class CommandType(IntEnum):
    NONE = 0
    GPIO_SET = 10
    GPIO_SET_HIGH_Z = 11
    GPIO_GET = 12
    UART = 20
    I2C_SET_SPEED = 30
    I2C_SET_ADDRESS = 31
    I2C_SET_TIMEOUT = 32
    I2C_READ = 33
    I2C_WRITE = 34
    SPI = 40


class CommandStatus(IntEnum):
    OK = 0
    GENERIC_ERROR = 1
    PARAMETER_ERROR = 2
    I2C_ERROR = 3
