add_library(dcm_app_bsp STATIC
    dcm_app_bsp.c
)

target_include_directories(dcm_app_bsp PUBLIC .)
target_link_libraries(dcm_app_bsp
    PUBLIC
        app
        core
        driver
        st_l4_support
    INTERFACE
        freertos_kernel
)
