import usb.core
import usb.util

dev = usb.core.find(idVendor=0xcafe, idProduct=0x4014)

if dev is None:
    raise ValueError('Device not found')

# dev.set_configuration()

# try:
#     dev.detach_kernel_driver(0)
# except:
#     pass

cfg = dev.get_active_configuration()
intf = cfg[(1, 0)]

ep_bulk_out = usb.util.find_descriptor(
    intf,
    custom_match = (
        lambda e:
            usb.util.endpoint_direction(e.bEndpointAddress) == usb.util.ENDPOINT_OUT and usb.util.endpoint_type(e.bmAttributes) == usb.util.ENDPOINT_TYPE_BULK
        )
    )

print("Testing bulk endpoint")

data = bytes([0xff, 0xff, 0xff, 0xff])
# data = bytes([0x00, 0x00, 0x00, 0x00])

ep_bulk_out.write(data)

# TIMEOUT_MS = 100
# ep_bulk_out.write(data, TIMEOUT_MS)

print("Done!")
