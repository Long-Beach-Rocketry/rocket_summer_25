set(EXECUTABLE sim)

add_executable_for(NATIVE ${EXECUTABLE} ""
    main.c
)

target_link_libraries_for(NATIVE ${EXECUTABLE}
    PRIVATE
        app
        core
        driver
)

add_executable_for(ARM32 ${EXECUTABLE} ""
    main.c
)

target_link_libraries_for(ARM32 ${EXECUTABLE}
    PRIVATE
        core
        driver
)

configure_file(test_data.log test_data.log COPYONLY)