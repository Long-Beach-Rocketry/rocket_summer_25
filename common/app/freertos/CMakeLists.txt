file(GLOB_RECURSE FREERTOS_APP_SRCS "*.c")

add_library(freertos_app STATIC
    ${FREERTOS_APP_SRCS}
)

target_include_directories(freertos_app PUBLIC
    cli
    lbr_net
    mission
    timeout
)

target_link_libraries(freertos_app PUBLIC
    core
    driver
    generic_app
    freertos_kernel
)