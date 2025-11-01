import struct

from command import Command
from defs import CommandStatus, CommandType


class i2c_set_address(Command):
    def __init__(self, address: int):
        self.address = address

    def write_payload(self):
        return struct.pack("<BB", CommandType.I2C_SET_ADDRESS, self.address)


class i2c_check_ack(Command):
    def write_payload(self):
        return struct.pack("<BI", CommandType.I2C_READ, 1)

    def read_length(self) -> int:
        return 1

    def parse_response(self, response: bytes, expected_status: None | int = None):
        status = response[0]
        return status == CommandStatus.OK


class i2c_read(Command):
    def __init__(self, length: int):
        self.length = length

    def write_payload(self):
        return struct.pack("<BI", CommandType.I2C_READ, self.length)

    def read_length(self) -> int:
        return 1 + self.length

    def parse_response(self, response: bytes, expected_status: None | int = None):
        status, *payload = response
        if expected_status is not None and status != expected_status:
            raise RuntimeError("Incorrect status")
        if len(payload) != self.length:
            raise RuntimeError(f"Incorrect read length {len(payload)}, expected {self.length}")
        return payload
