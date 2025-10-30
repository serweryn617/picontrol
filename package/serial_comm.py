import serial.tools.list_ports
import defs
import struct

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

    # TODO: move set/get pins to separate class
    def set_pins(self, pin_mask: int, pin_values: int):
        payload = struct.pack('<BII', defs.CommandType.GPIO_SET, pin_mask, pin_values)

        self.send_data(payload)

    def get_pins(self):
        payload = struct.pack('<B', defs.CommandType.GPIO_GET)

        self.serial.write(payload)
        result = self.serial.read(4)
        result = struct.unpack('<I', result)[0]
        return result

    def send_data(self, payload, expected_status=None):
        self.serial.write(payload)
        self.serial.flush()
        result = self.serial.read(1)
        result = struct.unpack('<B', result)[0]

        if expected_status is not None and result != expected_status:
            raise RuntimeError("Incorrect status")

        # self.serial.reset_input_buffer()
        # self.serial.reset_output_buffer()

        return result
