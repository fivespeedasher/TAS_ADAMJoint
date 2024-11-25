#include "include/BulbConstants.h"
#include "include/ControlRequest.h"
#include <iostream>
#include <vector>
#include <termios.h> //TCSANOW
#include <string.h>
#include <fcntl.h> // fcntl

using namespace std;

int main() {
    const char* remote_host = "192.168.3.1";
    int port = 10125;

    // 基类ADAM: 放置串口连接的参数
    ADAM adamPort2(remote_host, port);
    adamPort2.connect(false); // 不调试状态下连接
    // 初始化通道
    int slave_ID_1 = 1; // 从机地址
    int totalCH_1 = 8; // 从机通道总数
    int slave_ID_2 = 2;
    int totalCH_2 = 8;
    int slave_ID_3 = 3;
    int totalCH_3 = 8;
    int slave_ID_4 = 4;
    int slave_ID_5 = 5;
    int slave_ID_6 = 6;
    int totalDI_4 = 16; // 从机DI总数
    int totalDI_5 = 16;
    int totalCH_6 = 8;
    int duty_cycles = 0.5; // 脉冲占空比

    ADAM4068 adam_1(adamPort2, slave_ID_1, totalCH_1);
    ADAM4068 adam_2(adamPort2, slave_ID_2, totalCH_2);
    ADAM4068 adam_3(adamPort2, slave_ID_3, totalCH_3);
    ADAM4051 adam_4(adamPort2, slave_ID_4, totalDI_4);
    ADAM4051 adam_5(adamPort2, slave_ID_5, totalDI_5);
    ADAM4168 adam_6(adamPort2, slave_ID_6, totalCH_6, duty_cycles);

    ControlRequest controller(adam_1, adam_2, adam_3, adam_4, adam_5, adam_6);
    
    // 测试ADAM所有设备
    cout << "测试警示灯" << endl;
    controller.controlWarningLight(true);
    sleep(2);
    controller.controlWarningLight(false);

    cout << "测试状态灯" << endl;
    vector<int> colors = {STATUS_LIGHT_YELLOW, STATUS_LIGHT_BLUE, STATUS_LIGHT_RED, STATUS_LIGHT_WHITE, STATUS_LIGHT_GREEN};
    for(int c : colors) {
        controller.controlStatusLight(c);
        controller.stopStatusLight();
        sleep(1);
    }
    
    cout << "测试舱内灯" << endl;
    controller.controlCabinLight(true);
    sleep(2);
    controller.controlCabinLight(false);

    cout << "测试行车灯" << endl;
    controller.controlDrivingLight(true);
    sleep(2);
    controller.controlDrivingLight(false);
    
    cout << "测试转向灯" << endl;
    controller.controlTurningLight(LEFT);
    sleep(3);
    controller.controlTurningLight(RIGHT);

    cout << "测试近光灯" << endl;
    controller.controlNearLight(true);
    sleep(2);
    controller.controlNearLight(false);

    cout << "测试远光灯" << endl;
    controller.controlFarLight(true);
    sleep(2);
    controller.controlFarLight(false);

    cout << "测试倒车灯" << endl;
    sleep(2);
    controller.controlReversingLight(true);
    sleep(2);
    controller.controlReversingLight(false);

    cout << "测试刹车灯" << endl;
    controller.controlBrakeLight(true);
    sleep(2);
    controller.controlBrakeLight(false);

    cout << "测试喇叭" << endl;
    controller.controlHorn(HORN_1);
    sleep(4);
    controller.stopHorn();

    cout << "测试传送带" << endl;
    controller.controlDeliverToLeft(FRONT); // 前辊筒
    controller.controlDeliverToLeft(REAR); // 后辊筒
    sleep(3);
    controller.stopDeliver();
    controller.controlDeliverToRight(FRONT); // 后辊筒
    controller.controlDeliverToRight(REAR); // 后辊筒
    sleep(3);
    controller.stopDeliver();


    cout << "测试卷帘门" << endl;
    controller.downLeftRollingDoor();
    sleep(35);
    controller.riseLeftRollingDoor();
    sleep(35);
    controller.downRightRollingDoor();
    sleep(35);
    controller.riseRightRollingDoor();
    sleep(35);

    return 0;
}