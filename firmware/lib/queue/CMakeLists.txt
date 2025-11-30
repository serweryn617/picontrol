add_library(queue STATIC
    src/queue.cpp
)

target_include_directories(queue PUBLIC
    inc/
)

target_link_libraries(queue PRIVATE
    pico_stdlib
)