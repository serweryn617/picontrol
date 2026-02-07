import serial.tools.list_ports

from picontrol.exceptions import DeviceNotFoundError


def interface_number_from_hwid(hwid: str) -> int:
    for part in hwid.split():
        if part.startswith("LOCATION="):
            return int(part.split(".")[-1])
    raise DeviceNotFoundError(f"Location not found in hwid: {hwid}")


def find_port(vid: int, pid: int, interface: int):
    for port in serial.tools.list_ports.comports():
        if port.vid == vid and port.pid == pid and interface_number_from_hwid(port.hwid) == interface:
            return port.device  # e.g. '/dev/ttyACM0' or 'COM3'
    raise DeviceNotFoundError("USB device not found")
