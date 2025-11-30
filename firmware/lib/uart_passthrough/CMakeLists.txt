add_library(uart_passthrough STATIC
    src/uart_passthrough.cpp
)

target_include_directories(uart_passthrough PUBLIC
    inc/
)

target_link_libraries(uart_passthrough
PUBLIC
    pico_stdlib
    uart_driver
    tinyusb
)
