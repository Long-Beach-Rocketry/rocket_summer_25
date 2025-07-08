
add_library(math STATIC
    matrix.c
    quaternion.c
)

target_include_directories(math PUBLIC .)
target_link_libraries(math PUBLIC m)

add_tests(math
    quaternion_test
)
