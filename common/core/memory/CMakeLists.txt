add_library(memory STATIC
    dalloc.c
    prealloc.c
)

target_include_directories(memory PUBLIC .)
target_link_libraries(memory PUBLIC
    error
)

add_tests(memory
    alloc_test
)