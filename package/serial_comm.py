import serial.tools.list_ports
import defs
import struct

def find_cdc_port(vid, pid):
    for port in serial.tools.list_ports.comports():
        if port.vid == vid and port.pid == pid:
            return port.device  # e.g. '/dev/ttyACM0' or 'COM3'
    raise RuntimeError("USB device not found")

class CdcGpioController:
    def __init__(self, vid=defs.VENDOR_ID, pid=defs.PRODUCT_ID):
        self.port = find_cdc_port(vid, pid)

    def set_pins(self, pin_mask: int, pin_values: int):
        """
        Send a GPIO command with:
        [command_type(1 byte), pin_mask(4 bytes), pin_values(4 bytes)]
        """
        payload = struct.pack('<BII', defs.CommandType.GPIO_SET, pin_mask, pin_values)

        ser = serial.Serial(self.port, 115200, timeout=1, write_timeout=1)
        ser.write(payload)
        ser.close()

    def get_pins(self):
        payload = struct.pack('<B', defs.CommandType.GPIO_GET)

        ser = serial.Serial(self.port, 115200, timeout=1, write_timeout=1)
        ser.write(payload)
        result = ser.read(4)
        result = struct.unpack('<I', result)[0]
        ser.close()
        return result
