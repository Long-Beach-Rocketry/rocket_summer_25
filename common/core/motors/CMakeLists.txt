
add_library(motors STATIC 
    #encoder/drv8242_enc.c 
    # motor_control/dcm_control.c
    # DC/fake_dcm.c
    # encoder/fake_enc.c
    motor_control/fake_dcm_control.c
    DC/st_dcm.c
    encoder/st_enc.c
)

target_include_directories(motors PUBLIC 
    encoder
    DC
    motor_control
    driver
)

target_link_libraries(motors PUBLIC 
    driver

) 

add_tests(motors
    drv8242_dcm_test
    drv8242_enc_test
    dcm_control_test
)
