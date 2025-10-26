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
// USB HID
//--------------------------------------------------------------------+

// Invoked when received GET_REPORT control request
// Application must fill buffer report's content and return its length.
// Return zero will cause the stack to STALL request
uint16_t tud_hid_get_report_cb(uint8_t itf, uint8_t report_id, hid_report_type_t report_type, uint8_t* buffer, uint16_t reqlen)
{
  // TODO not Implemented
  (void) itf;
  (void) report_id;
  (void) report_type;
  (void) buffer;
  (void) reqlen;

  return 0;
}

// Invoked when received SET_REPORT control request or
// received data on OUT endpoint ( Report ID = 0, Type = 0 )
void tud_hid_set_report_cb(uint8_t itf, uint8_t report_id, hid_report_type_t report_type, uint8_t const* buffer, uint16_t bufsize)
{
  // This example doesn't use multiple report and report ID
  (void) itf;
  (void) report_id;
  (void) report_type;

  // echo back anything we received from host, converted to uppercase
  uint8_t uppercase[64] = {};
  uint8_t count = bufsize > 64 ? 64 : bufsize;
  for (uint8_t i = 0; i < count; i++) {
    uint8_t elem = buffer[i];
    if (elem >= 97 && elem <= 122) {
        elem -= 32;
    }
    uppercase[i] = elem;
  }
  tud_hid_report(0, uppercase, count);
}

//--------------------------------------------------------------------+
// Vendor
//--------------------------------------------------------------------+

void tud_vendor_rx_cb(uint8_t itf, uint8_t const* buffer, uint16_t bufsize)
{
  printf("tud_vendor_rx_cb\n");
  printf("  got %u bytes\n  ", bufsize);
  for (size_t i = 0; i < bufsize; i++)
  {
    printf("%u ", buffer[i]);
  }
  printf("\n");

  // TODO: don't overwrite
  memcpy(global_tinyusb_context.command, buffer, bufsize);
  global_tinyusb_context.size = bufsize;
  global_tinyusb_context.ready = true;
  tud_vendor_read_flush();
}

void tud_vendor_tx_cb(uint8_t itf, uint32_t sent_bytes)
{
}

//--------------------------------------------------------------------+
// CDC
//--------------------------------------------------------------------+

void tud_cdc_rx_cb(uint8_t itf)
{
  uint8_t buffer[64];
  uint32_t bufsize = tud_cdc_read(buffer, sizeof(buffer));

  printf("tud_cdc_rx_cb\n");
  printf("  got %u bytes\n  ", bufsize);
  for (size_t i = 0; i < bufsize; i++)
  {
    printf("%u ", buffer[i]);
  }
  printf("\n");

  if (global_tinyusb_context.ready) {
    printf("TinyUSB not ready!\n");
    return;
  }

  memcpy(global_tinyusb_context.command, buffer, bufsize);
  global_tinyusb_context.size = bufsize;
  global_tinyusb_context.ready = true;
}
