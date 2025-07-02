#include <gtest/gtest.h>

extern "C"
{
#include "dcm_control.h"
#include "fake_enc.h"
#include "gpio.h"
#include "pwm.h"
#include "st_dcm.h"
}

class Drv8242EncTest : public testing::Test
{
public:
    QEnc qenc;
    DCMotor motor;
    MotorRotoationCtrler controller;
    DCPosControl motor_control;
    Gpio brake, direction;
    Pwm pwm;
    DCM_Control st_control;
};

TEST_F(Drv8242EncTest, InitTest)
{
    QEnc_Init(&qenc, &controller, &motor);
    StDcmInit(&motor, &motor_control, &brake, &direction, &pwm);
    EXPECT_EQ(st_control.pusle_per_rev, 400);
    EXPECT_EQ(qenc.getTicks(&qenc), 0);
    DcmControlCommand(&st_control, true, 30000);
    controller.cmd = true;
    DcmControlCommand(&st_control, true, 30000);  //segfault
    EXPECT_EQ(qenc.getTicks(&qenc), 0);
    DcmControlUpdate(&st_control);
    EXPECT_EQ(fake_increment(&qenc, 30000), 30000);
    DcmControlUpdate(&st_control);
    EXPECT_EQ(qenc.getTicks(&qenc), 30000);
}
