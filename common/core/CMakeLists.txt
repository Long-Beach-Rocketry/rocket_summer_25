
add_library(core INTERFACE)

target_link_libraries(core INTERFACE
    cli
    comms
    container
    gnc
    error
    lbr_net
    math
    memory
    periph
    string
    switches
    timeout
    motors
)

target_link_libraries(core INTERFACE driver )

add_subdirectory(cli)
add_subdirectory(comms)
add_subdirectory(container)
add_subdirectory(gnc)
add_subdirectory(error)
add_subdirectory(lbr_net)
add_subdirectory(math)
add_subdirectory(memory)
add_subdirectory(periph)
add_subdirectory(string)
add_subdirectory(switches)
add_subdirectory(timeout)
add_subdirectory(motors)

