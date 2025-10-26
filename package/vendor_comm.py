import usb.core
import usb.util
import struct
import defs


class VendorGpioController:
    def __init__(self):
        self.dev = usb.core.find(idVendor=defs.VENDOR_ID, idProduct=defs.PRODUCT_ID)
        if self.dev is None:
            raise ValueError("Device not found")

        # self.dev.set_configuration()
        cfg = self.dev.get_active_configuration()
        intf = cfg[(1, 0)]

        # if self.dev.is_kernel_driver_active(intf.bInterfaceNumber):
        #     self.dev.detach_kernel_driver(intf.bInterfaceNumber)

        # usb.util.claim_interface(self.dev, intf.bInterfaceNumber)

        ep_out = usb.util.find_descriptor(
            intf,
            custom_match=lambda e: usb.util.endpoint_direction(e.bEndpointAddress) == usb.util.ENDPOINT_OUT
                                and usb.util.endpoint_type(e.bmAttributes) == usb.util.ENDPOINT_TYPE_BULK
        )
        if ep_out is None:
            raise ValueError("Bulk OUT endpoint not found")
        self._ep_out = ep_out

    def set_pins(self, pin_mask: int, pin_values: int):
        """
        Send a GPIO command with:
        [command_type(1 byte), pin_mask(4 bytes), pin_values(4 bytes)]
        """
        # Pack 1 byte + 2 uint32 little-endian
        payload = struct.pack('<BII', defs.CommandType.GPIO, pin_mask, pin_values)
        self._ep_out.write(payload)
