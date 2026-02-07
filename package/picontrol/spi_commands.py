import struct

from picontrol.command import Command
from picontrol.defs import CommandType
from picontrol.exceptions import CommandResponseError


class SpiSetSpeed(Command):
    def __init__(self, speed: int):
        self.speed = speed

    def write_payload(self):
        return struct.pack("<BI", CommandType.SPI_SET_SPEED, self.speed)


class SpiCsSelect(Command):
    def write_payload(self):
        return struct.pack("<B", CommandType.SPI_CS_SELECT)


class SpiCsDeselect(Command):
    def write_payload(self):
        return struct.pack("<B", CommandType.SPI_CS_DESELECT)


class SpiRead(Command):
    def __init__(self, length: int):
        self.length = length

    def write_payload(self):
        return struct.pack("<BI", CommandType.SPI_READ, self.length)

    def read_length(self) -> int:
        return 1 + self.length

    def parse_response(self, response: bytes, expected_status: None | int = None):
        status, *payload = response
        if expected_status is not None and status != expected_status:
            raise CommandResponseError("Incorrect status", status)
        if len(payload) != self.length:
            raise CommandResponseError(f"Incorrect read length {len(payload)}, expected {self.length}", status)
        return payload


class SpiWrite(Command):
    def __init__(self, data: bytes):
        self.data = data
        self.length = len(data)

    def write_payload(self):
        header = struct.pack("<B", CommandType.SPI_WRITE)
        return header + self.data
