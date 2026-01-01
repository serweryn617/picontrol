import struct

import defs
import serial.tools.list_ports
from command import Command

TRANSACTION_MAGIC = 0xBADCAB1E
TRANSACTION_HEADER_SIZE = 8
MAX_WRITE_LENGTH = 9 * 1024 - TRANSACTION_HEADER_SIZE
MAX_READ_LENGTH = 9 * 1024


def interface_number_from_hwid(hwid: str) -> int:
    for part in hwid.split():
        if part.startswith("LOCATION="):
            return int(part.split(".")[-1])
    raise RuntimeError(f"Location not found in hwid: {hwid}")


def find_cdc_port(vid: int, pid: int, interface: int):
    for port in serial.tools.list_ports.comports():
        if port.vid == vid and port.pid == pid and interface_number_from_hwid(port.hwid) == interface:
            return port.device  # e.g. '/dev/ttyACM0' or 'COM3'
    raise RuntimeError("USB device not found")


class SerialCommunicator:
    def __init__(self, vid=defs.VENDOR_ID, pid=defs.PRODUCT_ID, interface=defs.PICONTROL_INTERFACE_NUMBER):
        self.port = find_cdc_port(vid, pid, interface)

    def __enter__(self):
        self.serial = serial.Serial(self.port, 115200, timeout=1, write_timeout=1)
        return self

    def __exit__(self, exc_type, exc_value, exc_traceback):
        self.serial.close()

    def execute(self, command: Command):
        payload = command.write_payload()
        length = len(payload)
        read_length = command.read_length()

        if length > MAX_WRITE_LENGTH:
            raise RuntimeError(
                f"Total command write length (including any header bytes) can't be longer than {MAX_WRITE_LENGTH/1024}kiB ({MAX_WRITE_LENGTH}B), was {length}B"
            )

        if read_length > MAX_READ_LENGTH:
            raise RuntimeError(
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
