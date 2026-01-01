import struct

from command import Command
from defs import CommandStatus, CommandType


class spi_set_speed(Command):
    def __init__(self, speed: int):
        self.speed = speed

    def write_payload(self):
        return struct.pack("<BI", CommandType.SPI_SET_SPEED, self.speed)


class spi_cs_select(Command):
    def write_payload(self):
        return struct.pack("<B", CommandType.SPI_CS_SELECT)


class spi_cs_deselect(Command):
    def write_payload(self):
        return struct.pack("<B", CommandType.SPI_CS_DESELECT)


class spi_read(Command):
    def __init__(self, length: int):
        self.length = length

    def write_payload(self):
        return struct.pack("<BI", CommandType.SPI_READ, self.length)

    def read_length(self) -> int:
        return 1 + self.length

    def parse_response(self, response: bytes, expected_status: None | int = None):
        status, *payload = response
        if expected_status is not None and status != expected_status:
            raise RuntimeError("Incorrect status")
        if len(payload) != self.length:
            raise RuntimeError(f"Incorrect read length {len(payload)}, expected {self.length}")
        return payload


class spi_write(Command):
    def __init__(self, data: bytes):
        self.data = data
        self.length = len(data)

    def write_payload(self):
        header = struct.pack("<B", CommandType.SPI_WRITE)
        return header + self.data
