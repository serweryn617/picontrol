import usb.core
import usb.util
import time

TOTAL_BYTES = 1 * 1024 #* 1024

dev = usb.core.find(idVendor=0xcafe, idProduct=0x4014)

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

TIMEOUT_MS = 1000


print("Testing bulk endpoint")

packet = ep_bulk_out.wMaxPacketSize or 64
chunk_size = max(packet * 16, 4096)
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
