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

    SPI_SET_SPEED = 40
    SPI_CS_SELECT = 41
    SPI_CS_DESELECT = 42
    SPI_READ = 43
    SPI_WRITE = 44
    FLASH_READ = 45
    FLASH_SECTOR_ERASE = 46
    FLASH_PAGE_PROGRAM = 47


class CommandStatus(IntEnum):
    OK = 0
    GENERIC_ERROR = 1
    PARAMETER_ERROR = 2
    I2C_ERROR = 3
