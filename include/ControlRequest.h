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
    ControlRequest(const char *ip, int port);
    ~ControlRequest();
    int connect();
    int disconnect();
    int controlWarningLight(bool state);
    int controlStatusLight(int color);
    int stopStatusLight();
    int controlDeliverToLeft(bool whichRoller);
    int controlDeliverToRight(bool whichRoller);
    int stopDeliver();
    int riseLeftRollingDoor();
    int riseRightRollingDoor();
    int downLeftRollingDoor();
    int downRightRollingDoor();
    int controlHorn(int horn);
    int stopHorn();
    vector<bool> readPositionSensor();
    int controlDrivingLight(bool state);
    int controlTurningLight(bool direction);
    int controlNearLight(bool state);
    int controlFarLight(bool state);
    int controlReversingLight(bool state);
    int controlBrakeLight(bool state);
    

// private:
    ADAM Port2;
    ADAM4068 adam_1;
    ADAM4068 adam_2;
    ADAM4068 adam_3;
    ADAM4051 adam_4;
    ADAM4051 adam_5;
    ADAM4168 adam_6;
    // 初始化通道与DIO
    int slave_ID_1 = 1; // 从机地址
    int totalCH_1 = 8; // 从机通道总数
    int slave_ID_2 = 2; // 从机地址
    int totalCH_2 = 8; // 从机通道总数
    int slave_ID_3 = 3; // 从机地址
    int totalCH_3 = 8; // 从机通道总数

    int slave_ID_4 = 4; // 从机地址
    int slave_ID_5 = 5;
    int slave_ID_6 = 6;
    int totalDI_4 = 16; // 从机DI总数
    int totalDI_5 = 16;
    int totalCH_6 = 8; // 从机通道总数
    int duty_cycles = 0.5; // 脉冲占空比
};
#endif // CONTROLREQUEST_H