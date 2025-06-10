#include <gtest/gtest.h>

extern "C"
{
#include "fake_dcm.h"
#include "fake_enc.h"
}

class Drv8242EncTest : public testing::Test
{
public:
    QEnc qenc;
    DCMotor motor;
    MotorRotoationCtrler controller;
    DCPosControl motor_control;
};

TEST_F(Drv8242EncTest, InitTest)
{
    QEnc_Init(&qenc, &controller, &motor);
    FakeInit(&motor, &motor_control);  //segfalt
    EXPECT_EQ(controller.ppr, 100);
    EXPECT_EQ(get_fake_ticks(&qenc), 0);
    EXPECT_EQ(controller.command_rotate(&controller, 10), false);
    controller.cmd = true;
    EXPECT_EQ(controller.command_rotate(&controller, 10), true);  //segfault
    EXPECT_EQ(controller.start_pos, 0);
    EXPECT_EQ(controller.update(&controller), true);  //NANI??
    EXPECT_EQ(fake_increment(&qenc, 10000), 10000);
    EXPECT_EQ(controller.update(&controller), false);
}
