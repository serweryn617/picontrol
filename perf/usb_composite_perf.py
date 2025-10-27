import serial.tools.list_ports
import time
import usb.core
import usb.util

VENDOR_ID = 0xcafe
PRODUCT_ID = 0x4015

TOTAL_BYTES = 1 * 1024 * 1024
TIMEOUT_MS = 1000

def find_cdc_port(vid, pid):
    for port in serial.tools.list_ports.comports():
        if port.vid == vid and port.pid == pid:
            return port.device  # e.g. '/dev/ttyACM0' or 'COM3'
    raise RuntimeError("USB device not found")


dev = usb.core.find(idVendor=VENDOR_ID, idProduct=PRODUCT_ID)

if dev is None:
    raise ValueError('Device not found')

# dev.set_configuration()

try:
    dev.detach_kernel_driver(0)
except:
    pass

cfg = dev.get_active_configuration()

intf = cfg[(1, 0)]
ep_bulk_out = usb.util.find_descriptor(
    intf,
    custom_match = (
        lambda e:
            usb.util.endpoint_direction(e.bEndpointAddress) == usb.util.ENDPOINT_OUT and usb.util.endpoint_type(e.bmAttributes) == usb.util.ENDPOINT_TYPE_BULK
        )
    )

intf = cfg[(0, 0)]
ep_intr_out = usb.util.find_descriptor(
    intf,
    custom_match = (
        lambda e:
            usb.util.endpoint_direction(e.bEndpointAddress) == usb.util.ENDPOINT_OUT and usb.util.endpoint_type(e.bmAttributes) == usb.util.ENDPOINT_TYPE_INTR
        )
    )



print("Testing bulk endpoint (64B packet)")

chunk_size = 64
chunk = bytes([0xC6]) * chunk_size

remaining = TOTAL_BYTES
total_written = 0

_ = ep_bulk_out.write(chunk[:min(remaining, chunk_size)], TIMEOUT_MS)

start = time.perf_counter()
while remaining > 0:
    n = min(remaining, chunk_size)
    wrote = ep_bulk_out.write(chunk[:n], TIMEOUT_MS)
    if wrote != n:
        raise IOError(f"Short write: expected {n}, wrote {wrote}")
    remaining -= wrote
    total_written += wrote
end = time.perf_counter()

elapsed = end - start
kib = total_written / (1024)
kibs = kib / elapsed
kbps = (total_written * 8) / (1_000 * elapsed)

print(f"Sent {kib:.2f} KiB in {elapsed:.4f} s")
print(f"Throughput: {kibs:.2f} KiB/s ({kbps:.2f} Kbit/s)")




print("Testing bulk endpoint (512kB packet)")

chunk_size = 512 * 1024
chunk = bytes([0xC6]) * chunk_size

remaining = TOTAL_BYTES
total_written = 0

_ = ep_bulk_out.write(chunk[:min(remaining, chunk_size)], TIMEOUT_MS)

start = time.perf_counter()
while remaining > 0:
    n = min(remaining, chunk_size)
    wrote = ep_bulk_out.write(chunk[:n], TIMEOUT_MS)
    if wrote != n:
        raise IOError(f"Short write: expected {n}, wrote {wrote}")
    remaining -= wrote
    total_written += wrote
end = time.perf_counter()

elapsed = end - start
kib = total_written / (1024)
kibs = kib / elapsed
kbps = (total_written * 8) / (1_000 * elapsed)

print(f"Sent {kib:.2f} KiB in {elapsed:.4f} s")
print(f"Throughput: {kibs:.2f} KiB/s ({kbps:.2f} Kbit/s)")



print()
print("Testing interrupt endpoint")

packet_size = ep_intr_out.wMaxPacketSize
report_id   = 0  # change if your HID uses Report IDs
payload     = bytes([report_id] + [0x55] * (packet_size - 1)) if report_id else bytes([0x55] * packet_size)

# --- benchmark loop ---
remaining = TOTAL_BYTES
sent = 0

# Warm-up
ep_intr_out.write(payload, TIMEOUT_MS)

start = time.perf_counter()
while remaining > 0:
    n = min(packet_size, remaining)
    ep_intr_out.write(payload[:n], TIMEOUT_MS)
    sent += n
    remaining -= n
end = time.perf_counter()

elapsed = end - start
kib = sent / (1024)
kibs = kib / elapsed
kbps = (sent * 8) / (1_000 * elapsed)

print(f"Sent {kib:.2f} KiB in {elapsed:.4f} s")
print(f"Throughput: {kibs:.2f} KiB/s ({kbps:.2f} Kbit/s)")



print()
print("Testing HID control endpoint")

packet_size = 64 # ep_intr_out.wMaxPacketSize
report_id   = 0  # change if your HID uses Report IDs
payload     = bytes([report_id] + [0x55] * (packet_size - 1)) if report_id else bytes([0x55] * packet_size)

# --- benchmark loop ---
remaining = TOTAL_BYTES
sent = 0

report_type = 0x02  # Output report
report_id = 0x00
wValue = (report_type << 8) | report_id
interface = 0

start = time.perf_counter()
while remaining > 0:
    n = min(packet_size, remaining)
    dev.ctrl_transfer(
        bmRequestType=0x21,
        bRequest=0x09,
        wValue=wValue,
        wIndex=interface,
        data_or_wLength=payload[:n]
    )

    sent += n
    remaining -= n
end = time.perf_counter()

elapsed = end - start
kib = sent / (1024)
kibs = kib / elapsed
kbps = (sent * 8) / (1_000 * elapsed)

print(f"Sent {kib:.2f} KiB in {elapsed:.4f} s")
print(f"Throughput: {kibs:.2f} KiB/s ({kbps:.2f} Kbit/s)")



print()
print("Testing CDC (64B packet)")

port = find_cdc_port(0xcafe, 0x4015)
chunk = bytes([0xC6]) * 64
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




print()
print("Testing CDC (512kB packet)")

port = find_cdc_port(0xcafe, 0x4015)
chunk = bytes([0xC6]) * 512 * 1024
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
