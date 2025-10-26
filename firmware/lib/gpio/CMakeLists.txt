add_library(gpio_driver STATIC
    src/gpio_driver.cpp
)

target_include_directories(gpio_driver PUBLIC
    inc/
)

target_link_libraries(gpio_driver
PUBLIC
    pico_stdlib
    defs
)
