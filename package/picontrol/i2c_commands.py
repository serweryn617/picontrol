import struct

from picontrol.command import Command
from picontrol.defs import CommandStatus, CommandType
from picontrol.exceptions import CommandResponseError


class I2cSetSpeed(Command):
    def __init__(self, speed: int):
        self.speed = speed

    def write_payload(self):
        return struct.pack("<BI", CommandType.I2C_SET_SPEED, self.speed)


class I2cSetAddress(Command):
    def __init__(self, address: int):
        self.address = address

    def write_payload(self):
        return struct.pack("<BB", CommandType.I2C_SET_ADDRESS, self.address)


class I2cSetTimeout(Command):
    def __init__(self, timeout: int):
        self.timeout = timeout

    def write_payload(self):
        return struct.pack("<BI", CommandType.I2C_SET_TIMEOUT, self.timeout)


class I2cRead(Command):
    def __init__(self, length: int):
        self.length = length

    def write_payload(self):
        return struct.pack("<BI", CommandType.I2C_READ, self.length)

    def read_length(self) -> int:
        return 1 + self.length

    def parse_response(self, response: bytes, expected_status: None | int = None):
        status, *payload = response
        if expected_status is not None and status != expected_status:
            raise CommandResponseError("Incorrect status", status)
        if len(payload) != self.length:
            raise CommandResponseError(f"Incorrect read length {len(payload)}, expected {self.length}", status)
        return payload


class I2cWrite(Command):
    def __init__(self, data: bytes):
        self.data = data
        self.length = len(data)

    def write_payload(self):
        header = struct.pack("<B", CommandType.I2C_WRITE)
        return header + self.data


class I2cCheckAck(Command):
    def write_payload(self):
        header = struct.pack("<B", CommandType.I2C_WRITE)
        return header + b"\x00"

    def parse_response(self, response: bytes, expected_status: None | int = None):
        status = response[0]
        return status == CommandStatus.OK
