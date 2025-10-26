add_library(i2c_driver STATIC
    src/i2c_driver.cpp
)

target_include_directories(i2c_driver PUBLIC
    inc/
)

target_link_libraries(i2c_driver
PUBLIC
    pico_stdlib
    hardware_i2c
)
