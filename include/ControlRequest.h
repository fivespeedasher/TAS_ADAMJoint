/**
 * @file ControlRequest.h
 * @author Jushawn
 * @brief 作为ADAM控制的上层封装
 * @version 0.1
 * @date 2024-11-13
 * 
 * @copyright Copyright (c) 2024
 * 
 */

#ifndef CONTROLREQUEST_H
#define CONTROLREQUEST_H
#include "BulbConstants.h"
#include "ADAM.h"

class ControlRequest {
public:
    ControlRequest();
    ~ControlRequest();
    int connect(const char *ip, int port);
    void controlDrivingLight(bool state);
    void controlReversingLight(bool state);
    void controlPulse(int channel, uint16_t pulse_times);

private:
    ADAM4068 *adam_1;
    ADAM4068 *adam_2;
    ADAM4068 *adam_3;
    ADAM4051 *adam_4;
    ADAM4051 *adam_5;
    ADAM4168 *adam_6;
};
