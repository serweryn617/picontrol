#include <stdlib.h>
#include <string.h>

#include "tusb.h"
#include "context.h"

// TinyUSB callback functions
extern tinyusb_context global_tinyusb_context;

//--------------------------------------------------------------------+
// Device callbacks
//--------------------------------------------------------------------+

// Invoked when device is mounted
void tud_mount_cb(void)
{
}

// Invoked when device is unmounted
void tud_umount_cb(void)
{
}

// Invoked when usb bus is suspended
// remote_wakeup_en : if host allow us  to perform remote wakeup
// Within 7ms, device must draw an average of current less than 2.5 mA from bus
void tud_suspend_cb(bool remote_wakeup_en)
{
  (void) remote_wakeup_en;
}

// Invoked when usb bus is resumed
void tud_resume_cb(void)
{
}

//--------------------------------------------------------------------+
// Vendor
//--------------------------------------------------------------------+

void tud_vendor_rx_cb(uint8_t itf, uint8_t const* buffer, uint16_t bufsize)
{
  // TODO: don't overwrite
  memcpy(global_tinyusb_context.command, buffer, bufsize);
  global_tinyusb_context.size = bufsize;
  global_tinyusb_context.ready = true;
  tud_vendor_read_flush();
}

void tud_vendor_tx_cb(uint8_t itf, uint32_t sent_bytes)
{
}
