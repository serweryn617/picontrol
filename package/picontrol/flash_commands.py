import struct

from picontrol.command import Command
from picontrol.defs import CommandType, FlashBusyTimeoutMs


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
    def __init__(self, address: int, flash_busy_timeout_ms: int = FlashBusyTimeoutMs.SECTOR_ERASE):
        self.address = address
        self.flash_busy_timeout_ms = flash_busy_timeout_ms

    def write_payload(self):
        return struct.pack("<BII", CommandType.FLASH_SECTOR_ERASE, self.address, self.flash_busy_timeout_ms)


class flash_chip_erase(Command):
    def __init__(self, flash_busy_timeout_ms: int = FlashBusyTimeoutMs.CHIP_ERASE):
        self.flash_busy_timeout_ms = flash_busy_timeout_ms

    def write_payload(self):
        return struct.pack("<BI", CommandType.FLASH_CHIP_ERASE, self.flash_busy_timeout_ms)


class flash_program(Command):
    def __init__(self, address: int, data: bytes, flash_busy_timeout_ms: int = FlashBusyTimeoutMs.PROGRAM):
        self.address = address
        self.data = data
        self.flash_busy_timeout_ms = flash_busy_timeout_ms

    def write_payload(self):
        header = struct.pack("<BII", CommandType.FLASH_PROGRAM, self.address, self.flash_busy_timeout_ms)
        return header + self.data
