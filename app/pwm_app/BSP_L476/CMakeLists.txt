add_library(pwm_app_bsp STATIC
    hal_timebase.c
    pwm_app_bsp.c
)

target_include_directories(pwm_app_bsp PUBLIC .)
target_link_libraries(pwm_app_bsp
    PUBLIC
        app
        core
        driver
    INTERFACE
        freertos_kernel
)
