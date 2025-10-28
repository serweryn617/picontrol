add_library(commands STATIC
    src/commands.cpp
)

target_include_directories(commands PUBLIC
    inc/
)

target_link_libraries(commands
PUBLIC
    pico_stdlib
    gpio_driver
    i2c_driver
)
