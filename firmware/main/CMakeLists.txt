add_executable(${PROJECT_NAME}
    src/main.cpp
)

target_link_libraries(${PROJECT_NAME}
    pico_stdlib
    tinyusb
    tinyusb_device
    tinyusb_board
    defs
    i2c_driver
    uart_driver
)

pico_add_extra_outputs(${PROJECT_NAME})
