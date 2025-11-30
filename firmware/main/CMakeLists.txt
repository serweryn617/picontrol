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
    i2c_driver
    pico_stdlib
    pico_multicore
    queue
    spi_driver
    tinyusb
    tinyusb_board
    tinyusb_device
    uart_driver
)

pico_add_extra_outputs(${PROJECT_NAME})
