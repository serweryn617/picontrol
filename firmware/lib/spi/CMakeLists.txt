add_library(spi_driver STATIC
    src/spi_driver.cpp
    src/spi_flash.cpp
)

target_include_directories(spi_driver PUBLIC
    inc/
)

target_link_libraries(spi_driver
PUBLIC
    pico_stdlib
    hardware_spi
)
