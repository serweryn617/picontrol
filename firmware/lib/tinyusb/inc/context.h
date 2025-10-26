#ifndef LIB_TINYUSB_CONTEXT_TYPE_H
#define LIB_TINYUSB_CONTEXT_TYPE_H

#include "pico/stdlib.h"

typedef struct {
    bool ready;
    uint8_t command[64];
    uint8_t size;
} tinyusb_context;

#endif  // LIB_TINYUSB_CONTEXT_TYPE_H
