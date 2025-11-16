import struct

import defs
import serial.tools.list_ports
from command import Command


def find_cdc_port(vid, pid):
    for port in serial.tools.list_ports.comports():
        if port.vid == vid and port.pid == pid:
            return port.device  # e.g. '/dev/ttyACM0' or 'COM3'
    raise RuntimeError("USB device not found")


class SerialCommunicator:
    def __init__(self, vid=defs.VENDOR_ID, pid=defs.PRODUCT_ID):
        self.port = find_cdc_port(vid, pid)

    def __enter__(self):
        self.serial = serial.Serial(self.port, 115200, timeout=1, write_timeout=1)
        return self

    def __exit__(self, exc_type, exc_value, exc_traceback):
        self.serial.close()

    def execute(self, command: Command):
        magic = 0xBADCAB1E
        payload = command.write_payload()
        length = len(payload)

        # print("Writing:")
        # print(struct.pack("<II", magic, length) + payload)

        self.serial.write(struct.pack("<II", magic, length) + payload)
        self.serial.flush()

        read_length = command.read_length()

        if read_length:
            response = self.serial.read(read_length)
            response = command.parse_response(response)

        self.serial.reset_input_buffer()
        self.serial.reset_output_buffer()

        if read_length:
            return response
