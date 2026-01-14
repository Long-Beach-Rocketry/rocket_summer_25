add_library(hal STATIC
    arm32_usart.c
    arm32_gpio.c
)

target_include_directories(hal PUBLIC
    bus
    memory
    .
)
target_link_libraries(hal PUBLIC
    core
)
