import serial.tools.list_ports
import time

def find_cdc_port(vid, pid):
    for port in serial.tools.list_ports.comports():
        if port.vid == vid and port.pid == pid:
            return port.device  # e.g. '/dev/ttyACM0' or 'COM3'
    raise RuntimeError("USB device not found")

TOTAL_BYTES = 1 * 1024 * 1024
TIMEOUT_MS = 1000

print("Testing CDC")

chunk = bytes([0xC6]) * 4096
port = find_cdc_port(0xcafe, 0x4015)
remaining = TOTAL_BYTES
total_written = 0

ser = serial.Serial(port, 115200)

ser.write(chunk)

start = time.perf_counter()
while remaining > 0:
    n = min(remaining, len(chunk))
    wrote = ser.write(chunk[:n])
    if wrote != n:
        raise IOError(f"Short write: expected {n}, wrote {wrote}")
    remaining -= wrote
    total_written += wrote
end = time.perf_counter()

ser.close()

elapsed = end - start
kib = total_written / (1024)
kibs = kib / elapsed
kbps = (total_written * 8) / (1_000 * elapsed)

print(f"Sent {kib:.2f} KiB in {elapsed:.4f} s")
print(f"Throughput: {kibs:.2f} KiB/s ({kbps:.2f} Kbit/s)")
