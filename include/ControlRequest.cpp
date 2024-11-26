// 以DB2接入六个ADAM模块，以DB1接显示屏
#include "ControlRequest.h"
#include <iostream>
using namespace std;
ControlRequest::ControlRequest( ADAM4068& adam_1, ADAM4068& adam_2, ADAM4068& adam_3, ADAM4051& adam_4, ADAM4051& adam_5, ADAM4168& adam_6)
    : adam_1(adam_1), adam_2(adam_2), adam_3(adam_3), 
    adam_4(adam_4), adam_5(adam_5), adam_6(adam_6) {}
ControlRequest::~ControlRequest() {}

// 控制警示灯
int ControlRequest::controlWarningLight(bool state) {
    adam_1.write_coil(WARNING_LIGHT, state);
    return 0;
}

/**
 * @brief 控制状态灯
 * 
 * @param color 可选 STATUS_LIGHT_YELLOW, STATUS_LIGHT_BLUE, STATUS_LIGHT_RED, STATUS_LIGHT_WHITE, STATUS_LIGHT_GREEN
 * @return int 
 */
int ControlRequest::controlStatusLight(int color) {
    stopStatusLight();
    adam_1.write_coil(color, true);
    return 0;
}
// 关闭状态灯
int ControlRequest::stopStatusLight() {
    adam_1.write_coil(STATUS_LIGHT_YELLOW, false);
    adam_1.write_coil(STATUS_LIGHT_BLUE, false);
    adam_1.write_coil(STATUS_LIGHT_RED, false);
    adam_1.write_coil(STATUS_LIGHT_WHITE, false);
    adam_1.write_coil(STATUS_LIGHT_GREEN, false);
    return 0;
}

// 控制舱内灯
int ControlRequest::controlCabinLight(bool state) {
    adam_1.write_coil(CABIN_LIGHT, state);
    return 0;
}

// FIXME 根据阻挡到位关DO
/**
 * @brief 控制货物往左输送
 * 
 * @param whichRoller 前辊筒0，后辊筒1
 * @return int 
 */
int ControlRequest::controlDeliverToLeft(bool whichRoller) {
    // 左方阻挡升起 0 1
    adam_2.write_coil(LEFT_BLOCKER_DOWN, false);
    adam_2.write_coil(LEFT_BLOCKER_UP, true);
    bool up_state = ControlRequest::readPositionSensor()[LEFT_BLOCKER_UP_POSITION];
    if(up_state == 1) {
        // 左方阻挡升起到位
        adam_2.write_coil(LEFT_BLOCKER_UP, false);
        if (whichRoller == FRONT) {
            // 前辊筒左送 1 0
            adam_2.write_coil(FRONT_ROLLER_LEFT, true);
            
        } else {
            // 后辊筒左送
            adam_2.write_coil(REAR_ROLLER_LEFT, true);
        }
    }
    else {return -1;}
    return 0;
}

/**
 * @brief 控制货物往右输送
 * 
 * @param whichRoller  前辊筒0，后辊筒1
 * @return int 
 */
int ControlRequest::controlDeliverToRight(bool whichRoller) {
    // 右方阻挡升起 0 1
    adam_2.write_coil(RIGHT_BLOCKER_DOWN, false);
    adam_2.write_coil(RIGHT_BLOCKER_UP, true);
    bool up_state = ControlRequest::readPositionSensor()[RIGHT_BLOCKER_UP_POSITION];
    if(up_state == 1) {
        // 左方阻挡升起到位
        adam_2.write_coil(RIGHT_BLOCKER_UP, false);
        if (whichRoller == FRONT) {
            // 前辊筒右送 1 0
            adam_2.write_coil(FRONT_ROLLER_RIGHT, true);
            
        } else {
            // 后辊筒右送
            adam_2.write_coil(REAR_ROLLER_RIGHT, true);
        }
    }
    else {return -1;}
    return 0;
}

// 停止辊筒输送
int ControlRequest::stopDeliver() {
    // 前辊筒停止
    adam_2.write_coil(FRONT_ROLLER_LEFT, false);
    adam_2.write_coil(FRONT_ROLLER_RIGHT, false);
    // 后辊筒停止
    adam_2.write_coil(REAR_ROLLER_LEFT, false);
    adam_2.write_coil(REAR_ROLLER_RIGHT, false);

    sleep(1);
    // 阻挡落下
    adam_2.write_coil(LEFT_BLOCKER_UP, false);
    adam_2.write_coil(RIGHT_BLOCKER_UP, false);

    adam_2.write_coil(LEFT_BLOCKER_DOWN, true);
    adam_2.write_coil(RIGHT_BLOCKER_DOWN, true);
    bool down_state_r = ControlRequest::readPositionSensor()[RIGHT_BLOCKER_DOWN_POSITION];
    bool down_state_l = ControlRequest::readPositionSensor()[LEFT_BLOCKER_DOWN_POSITION];
    if(down_state_r == 1 && down_state_l == 1) {
        // 左右阻挡落下到位
        adam_2.write_coil(LEFT_BLOCKER_DOWN, false);
        adam_2.write_coil(RIGHT_BLOCKER_DOWN, false);
    }
    else {return -1;}

    return 0;
}

// 升起左侧卷帘门
int ControlRequest::riseLeftRollingDoor() {
    adam_3.write_coil(LEFT_ROLLING_DOOR_DOWN, false);
    adam_3.write_coil(LEFT_ROLLING_DOOR_UP, true);
    // TODO 用到位传感器来停下IO输出
    return 0;
}

// 升起右侧卷帘门
int ControlRequest::riseRightRollingDoor() {
    adam_3.write_coil(RIGHT_ROLLING_DOOR_DOWN, false);
    adam_3.write_coil(RIGHT_ROLLING_DOOR_UP, true);
    // TODO 用到位传感器来停下IO输出
    return 0;
}

// 下放左侧卷帘门
int ControlRequest::downLeftRollingDoor() {
    adam_3.write_coil(LEFT_ROLLING_DOOR_UP, false);
    adam_3.write_coil(LEFT_ROLLING_DOOR_DOWN, true);
    // TODO 用到位传感器来停下IO输出
    return 0;
}

// 下放右侧卷帘门
int ControlRequest::downRightRollingDoor() {
    adam_3.write_coil(RIGHT_ROLLING_DOOR_UP, false);
    adam_3.write_coil(RIGHT_ROLLING_DOOR_DOWN, true);
    // TODO 用到位传感器来停下IO输出
    return 0;
}


/**
 * @brief 打开喇叭
 * 
 * @param horn 通道数可选HORN_1~4
 * @return int 
 */
int ControlRequest::controlHorn(int horn) {
    stopHorn();
    adam_3.write_coil(horn, true);
    return 0;
}
// 关闭喇叭
int ControlRequest::stopHorn() {
    adam_3.write_coil(HORN_1, false);
    adam_3.write_coil(HORN_2, false);
    adam_3.write_coil(HORN_3, false);
    adam_3.write_coil(HORN_3, false);
    return 0;
}

// 读取并打印到位传感器状态
vector<bool> ControlRequest::readPositionSensor() {
    vector<bool> positionSensor;
    adam_4.read_coils();
    cout << "Position Sensor : ";
    for (int i = 0; i < 16; i++) {
        positionSensor.push_back(adam_4.state_coils[i]);
        cout << adam_4.state_coils[i] << " "; // TODO 检查输出
    }
    cout << endl;
    return positionSensor;
}


/**
 * @brief 打开行车灯
 * 
 * @param state 0 关闭，1 打开
 */
int ControlRequest::controlDrivingLight(bool state) {
    adam_6.SetDO(RUNNING_LIGHT, state); 
    return 0;
}

/**
 * @brief 打开转向灯，闪烁三次
 * 
 * @param direction 0 左转向灯，1 右转向灯
 */
int ControlRequest::controlTurningLight(bool direction) {
    vector<int> PulseChannel;
    if (direction) {
        PulseChannel = {TURNING_RIGHT_LIGHT};
    } else {
        PulseChannel = {TURNING_LEFT_LIGHT};
    }
    adam_6.StartPulse(PulseChannel, BLINK); // 打开转向
    return 0;
}
// 打开近光灯,会自动关闭远光灯
int ControlRequest::controlNearLight(bool state) {
    adam_6.SetDO(FAR_LIGHT, false); // 关闭远光灯
    adam_6.SetDO(NEAR_LIGHT, state); // 打开近光灯
    return 0;
}
// 打开远光灯,会自动关闭近光灯
int ControlRequest::controlFarLight(bool state) {
    adam_6.SetDO(NEAR_LIGHT, false); // 关闭近光灯
    adam_6.SetDO(FAR_LIGHT, state); // 打开远光灯
    return 0;
}

// 打开倒车灯
int ControlRequest::controlReversingLight(bool state) {
    adam_6.SetDO(REVERSING_LIGHT, state); // 打开倒车灯
    return 0;
}
// 打开刹车灯
int ControlRequest::controlBrakeLight(bool state) {
    adam_6.SetDO(BRAKE_LIGHT, state); // 打开刹车灯
    return 0;
}

