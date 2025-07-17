add_library(servo_app_bsp STATIC
    servo_app_bsp.c
)

target_include_directories(servo_app_bsp PUBLIC .)
target_link_libraries(servo_app_bsp
    PUBLIC
        app
        core
        driver
    INTERFACE
        freertos_kernel
)
