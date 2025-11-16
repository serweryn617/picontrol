add_library(commands STATIC
    src/commands.cpp
    src/gpio.cpp
    src/spi.cpp
)

target_include_directories(commands PUBLIC
    inc/
)

target_link_libraries(commands
PUBLIC
    gpio_driver
    i2c_driver
    pico_stdlib
    spi_driver
    defs
)
