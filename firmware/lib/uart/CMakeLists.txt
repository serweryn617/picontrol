add_library(uart_driver STATIC
    src/uart_driver.cpp
)

target_include_directories(uart_driver PUBLIC
    inc/
)

target_link_libraries(uart_driver
PUBLIC
    pico_stdlib
    hardware_uart
)
