import struct
from dataclasses import dataclass

from defs import CommandStatus


@dataclass
class Command:
    def write_payload(self):
        raise NotImplementedError

    def read_length(self) -> int:
        return 1

    def parse_response(self, response: bytes, expected_status: None | int = CommandStatus.OK):
        status = struct.unpack("<B", response)[0]
        if expected_status is not None and status != expected_status:
            raise RuntimeError(f"Incorrect status {status}")
