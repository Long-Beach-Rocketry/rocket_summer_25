add_library(hal STATIC
    st_gpio.c
    st_gpio_fctry.c
    st_i2c.c
    st_i2c_fctry.c
    st_usart.c
    st_usart_fctry.c
    st_pwm.c
    # st_pwm_fctry.c
    st_spi.c
)

target_include_directories(hal PUBLIC
    bus
    io
    .
)
target_link_libraries(hal PUBLIC
    core
    st_l4_support
)
