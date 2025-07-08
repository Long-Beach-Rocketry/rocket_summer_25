
if (NOT "${FREERTOS_PORT}" STREQUAL "")
    add_library(freertos_config INTERFACE)
    # The config file directory
    target_compile_definitions(freertos_config INTERFACE 
        projCOVERAGE_TEST=0
    )
    target_link_libraries(freertos_config INTERFACE hal)

    add_subdirectory(FreeRTOS-Kernel)
endif()

if ("${TARGET_DEVICE}" MATCHES "NATIVE")
    # For Windows: Prevent overriding the parent project's compiler/linker settings
    set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
    add_subdirectory(googletest)
endif()

#probably should be changed later but just trying to make it work
add_subdirectory(nanopb)

