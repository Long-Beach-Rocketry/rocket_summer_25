
add_library(gnc STATIC
    acceleration.c
    altitude.c
    simple_kalman.c
)

target_include_directories(gnc PUBLIC .)
target_link_libraries(gnc PUBLIC math)