
add_library(container STATIC
    ring_buffer.c
)

target_include_directories(container PUBLIC .)
target_link_libraries(container PUBLIC memory)

add_tests(container
    ring_buffer_test
)
