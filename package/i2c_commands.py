from defs import CommandType
import struct

def make_i2c_set_address_command(address):
    return struct.pack('<BB', CommandType.I2C_SET_ADDRESS, address)

def make_i2c_read_command(length):
    return struct.pack('<BI', CommandType.I2C_READ, length)
