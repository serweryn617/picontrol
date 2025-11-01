import struct

from command import Command
from defs import CommandType


class gpio_set(Command):
    def __init__(self, pin_mask: int, pin_values: int):
        self.pin_mask = pin_mask
        self.pin_values = pin_values

    def write_payload(self):
        return struct.pack("<BII", CommandType.GPIO_SET, self.pin_mask, self.pin_values)


class gpio_get(Command):
    def write_payload(self):
        return struct.pack("<B", CommandType.GPIO_GET)

    def read_length(self) -> int:
        return 5

    def parse_response(self, response: bytes, expected_status: None | int = 0):
        status, pins_state = struct.unpack("<BI", response)[:2]
        if expected_status is not None and status != expected_status:
            raise RuntimeError("Incorrect status")
        return pins_state
