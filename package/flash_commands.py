import struct

from command import Command
from defs import CommandStatus, CommandType


class flash_read_status(Command):
    def write_payload(self):
        return struct.pack("<B", CommandType.FLASH_READ_STATUS)

    def read_length(self) -> int:
        return 1 + 3

    def parse_response(self, response: bytes, expected_status: None | int = None):
        status, *payload = response
        if expected_status is not None and status != expected_status:
            raise RuntimeError("Incorrect status")
        if len(payload) != 3:
            raise RuntimeError(f"Incorrect status length {len(payload)}, expected {self.length}")
        return payload


class flash_read(Command):
    def __init__(self, address: int, length: int):
        self.address = address
        self.length = length

    def write_payload(self):
        return struct.pack("<BII", CommandType.FLASH_READ, self.address, self.length)

    def read_length(self) -> int:
        return 1 + self.length

    def parse_response(self, response: bytes, expected_status: None | int = None):
        status, *payload = response
        if expected_status is not None and status != expected_status:
            raise RuntimeError("Incorrect status")
        if len(payload) != self.length:
            raise RuntimeError(f"Incorrect read length {len(payload)}, expected {self.length}")
        return payload


class flash_sector_erase(Command):
    def __init__(self, address: int):
        self.address = address

    def write_payload(self):
        return struct.pack("<BI", CommandType.FLASH_SECTOR_ERASE, self.address)


class flash_program(Command):
    def __init__(self, address: int, data: bytes):
        self.address = address
        self.data = data

    def write_payload(self):
        header = struct.pack("<BI", CommandType.FLASH_PROGRAM, self.address)
        return header + self.data
