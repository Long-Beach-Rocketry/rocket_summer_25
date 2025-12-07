add_library(uart_network_bsp STATIC
    uart_network_bsp.c
    hal_timebase.c
)

target_include_directories(uart_network_bsp PUBLIC .)
target_link_libraries(uart_network_bsp
    PUBLIC
        app
        core
        driver
    INTERFACE
        freertos_kernel
)
