if (TARGET_DEVICE MATCHES "NATIVE")
    add_subdirectory(platform/native)
elseif(TARGET_DEVICE MATCHES "ARM_LINUX")
    add_subdirectory(platform/arm_linux)
elseif (TARGET_DEVICE MATCHES "^(STM32F4)[0-9]+")
    add_subdirectory(platform/stm32f4)
elseif (TARGET_DEVICE MATCHES "^(STM32F7)[0-9]+")
    add_subdirectory(platform/stm32f7)
elseif (TARGET_DEVICE MATCHES "^(STM32H5)[0-9]+")
    add_subdirectory(platform/stm32h5)
elseif (TARGET_DEVICE MATCHES "^(STM32L4)[0-9]+")
    add_subdirectory(platform/stm32l4)
endif()

add_library(driver INTERFACE)

target_include_directories(driver INTERFACE
    bus
    memory
    io
)

target_link_libraries(driver INTERFACE
    hal
)