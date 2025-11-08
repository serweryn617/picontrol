add_executable(${PROJECT_NAME}
    src/main.cpp
)

target_link_libraries(${PROJECT_NAME}
    commands
    defs
    gpio_driver
    i2c_driver
    pico_stdlib
    spi_driver
    tinyusb
    tinyusb_board
    tinyusb_device
    uart_driver
)

pico_add_extra_outputs(${PROJECT_NAME})
