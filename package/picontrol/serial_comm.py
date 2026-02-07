import struct

import serial

import picontrol.defs
from picontrol.command import Command
from picontrol.exceptions import RequestedCommandError
from picontrol.ports import find_port

TRANSACTION_MAGIC = 0xBADCAB1E
TRANSACTION_HEADER_SIZE = 8
MAX_WRITE_LENGTH = 9 * 1024 - TRANSACTION_HEADER_SIZE
MAX_READ_LENGTH = 9 * 1024


class SerialCommunicator:
    def __init__(self, port: str):
        self.port = port

    def __enter__(self):
        self.connect()
        return self

    def __exit__(self, exc_type, exc_value, exc_traceback):
        self.disconnect()

    def connect(self):
        # NOTE: Some commands, for example flash chip erase take ~20s
        # TODO: Use specific timeouts per each command
        self.serial = serial.Serial(self.port, 115200, timeout=60, write_timeout=1)

    def disconnect(self):
        self.serial.close()

    def execute(self, command: Command):
        payload = command.write_payload()
        length = len(payload)
        read_length = command.read_length()

        if length > MAX_WRITE_LENGTH:
            raise RequestedCommandError(
                f"Total command write length (including any header bytes) can't be longer than {MAX_WRITE_LENGTH/1024}kiB ({MAX_WRITE_LENGTH}B), was {length}B"
            )

        if read_length > MAX_READ_LENGTH:
            raise RequestedCommandError(
                f"Total command read length (including any status bytes) can't be longer than {MAX_READ_LENGTH/1024}kiB ({MAX_READ_LENGTH}B), was {read_length}B"
            )

        transaction_header = struct.pack("<II", TRANSACTION_MAGIC, length)
        assert len(transaction_header) == TRANSACTION_HEADER_SIZE, "Defined value doesn't match"
        self.serial.write(transaction_header + payload)
        self.serial.flush()

        if read_length:
            response = self.serial.read(read_length)
            response = command.parse_response(response)

        self.serial.reset_input_buffer()
        self.serial.reset_output_buffer()

        if read_length:
            return response


class PiControlComm(SerialCommunicator):
    def __init__(self):
        picontrol_port = find_port(picontrol.defs.VENDOR_ID, picontrol.defs.PRODUCT_ID, picontrol.defs.PICONTROL_INTERFACE_NUMBER)
        super().__init__(picontrol_port)
