add_library(tinyusb STATIC
    src/tinyusb_cb.c
    src/tinyusb.cpp
    src/usb_descriptors.c
)

target_include_directories(tinyusb PUBLIC
    inc/
)

target_link_libraries(tinyusb
PUBLIC
    tinyusb_device
PRIVATE
    pico_stdlib
    tinyusb_board
)
