add_executable(${PROJECT_NAME}
    src/main.cpp
    src/core1_main.cpp
)

target_include_directories(${PROJECT_NAME} PRIVATE
    inc/
)

target_link_libraries(${PROJECT_NAME}
    commands
    defs
    gpio_driver
    hardware_dma
    i2c_driver
    pico_multicore
    pico_stdlib
    queue
    spi_driver
    tinyusb
    tinyusb_board
    tinyusb_device
    uart_driver
    uart_passthrough
)

pico_add_extra_outputs(${PROJECT_NAME})
