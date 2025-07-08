# CountDebounce logic as a static library
add_library(switches STATIC
    count_debounce.c
)

target_include_directories(switches PUBLIC .)
target_link_libraries(switches PUBLIC driver)

# GoogleTest unit test for CountDebounce
add_tests(switches
    count_debounce_test
)
