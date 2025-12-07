
#pragma once

#include "controller.h"

typedef struct
{
    bool enable;
    float angle;
} pb_cmd_msg;

typedef struct
{
    bool running;
} pb_telem_msg;

typedef struct
{
    pb_cmd_msg cmd;
    pb_telem_msg telem;
    Packet cmd_packet;
    Packet telem_packet;
    Controller ctrl;

} MotorCtrler;

bool MotorCtrlerRotate(MotorCtrler* motor, float degrees)
{

    motor->cmd.enable = true;
    motor->cmd.angle = degrees;
    EXIT_IF(!ControllerCommand(&motor->ctrl, &motor->cmd_packet,
                               &motor->telem_packet),
            false);

    while (motor->telem.running)
    {
        EXIT_IF(!ControllerCommand(&motor->ctrl, &motor->cmd_packet,
                                   &motor->telem_packet),
                false);
    }
    motor->cmd.enable = false;
    EXIT_IF(!ControllerCommand(&motor->ctrl, &motor->cmd_packet,
                               &motor->telem_packet),
            false);

    return true;
}

/*
//send
uint8_t message_buffer[MAX_RECEIVE_BUF_SIZE];
uint8_t data[MAX_RECEIVE_BUF_SIZE];
uint8_t pb_size = cmd_message->get_size(cmd_message);
cmd_message->get_buf(cmd_message, data, MAX_RECEIVE_BUF_SIZE);
bus.pack(&bus, message_buffer, MAX_RECEIVE_BUF_SIZE, target_address, data,
         pb_size);

//receive
uint8_t size = bus.get_package_size(&bus);
uint8_t data[size];
cmd_message->parse(cmd_message, data, size);
*/