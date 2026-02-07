import struct
from dataclasses import dataclass

from picontrol.defs import CommandStatus, CommandType
from picontrol.exceptions import CommandResponseError


@dataclass
class Command:
    def write_payload(self):
        raise NotImplementedError

    def read_length(self) -> int:
        return 1

    def parse_response(self, response: bytes, expected_status: None | int = CommandStatus.OK):
        status = struct.unpack("<B", response)[0]
        if expected_status is not None and status != expected_status:
            # TODO: Print better error info
            raise CommandResponseError(f"Incorrect status {status}", status)


class EnterBootsel(Command):
    def write_payload(self):
        return struct.pack("<B", CommandType.ENTER_BOOTSEL)

    def read_length(self) -> int:
        return 0
