import usb.core
import usb.util

dev = usb.core.find(idVendor=0xcafe, idProduct=0x4014)

if dev is None:
    raise ValueError('Device not found')

cfg = dev.get_active_configuration()
intf = cfg[(1, 0)]

ep_bulk_out = usb.util.find_descriptor(
    intf,
    custom_match = (
        lambda e:
            usb.util.endpoint_direction(e.bEndpointAddress) == usb.util.ENDPOINT_OUT and usb.util.endpoint_type(e.bmAttributes) == usb.util.ENDPOINT_TYPE_BULK
        )
    )

print("Setting GPIOs")

data = bytes([0x01, 0x02, 0x00, 0x00, 0x00])
ep_bulk_out.write(data)

print("Done!")
