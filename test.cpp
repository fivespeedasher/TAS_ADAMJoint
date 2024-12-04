#include "include/BulbConstants.h"
#include "include/ControlRequest.h"
#include "include/LEDScreenController.h"
#include "include/utils.h"
#include <iostream>
#include <vector>
#include <termios.h> //TCSANOW
#include <string.h>
#include <fcntl.h> // fcntl

using namespace std;

int main() {
    const char* remote_host = "192.168.3.1";
    int port_1 = 10123;
    int port_2 = 10125;
    bool setDebug = false;

    // LED屏初始化
    LEDScreenController LEDcontroller(remote_host, port_1, setDebug);
    LEDcontroller.connect();
    LEDcontroller.initLED();

    // ADAM模块初始化
    // 基类ADAM: 放置串口连接的参数
    ADAM adamPort2(remote_host, port_2);
    adamPort2.connect(setDebug); // 不调试状态下连接
    // 初始化通道
    ADAM4068 adam_1(adamPort2, slave_ID_1, totalCH_1);
    ADAM4068 adam_2(adamPort2, slave_ID_2, totalCH_2);
    ADAM4068 adam_3(adamPort2, slave_ID_3, totalCH_3);
    ADAM4051 adam_4(adamPort2, slave_ID_4, totalDI_4);
    ADAM4051 adam_5(adamPort2, slave_ID_5, totalDI_5);
    ADAM4168 adam_6(adamPort2, slave_ID_6, totalCH_6, duty_cycles);

    // 控制请求类初始化
    ControlRequest controller(adam_1, adam_2, adam_3, adam_4, adam_5, adam_6);

    // 测试300轮
    for(int i = 0; i < 300; i++) {
        cout << "Testing round " << i+1 << endl;
        
        // 测试ADAM所有设备
        cout << "测试警示灯" << endl;
        LEDcontroller.writeText("警示灯测试", 0);
        controller.controlWarningLight(true);
        sleep(2);
        controller.controlWarningLight(false);

        cout << "测试状态灯" << endl;
        LEDcontroller.writeText("测试状态灯", 0);
        vector<int> colors = {STATUS_LIGHT_YELLOW, STATUS_LIGHT_BLUE, STATUS_LIGHT_RED, STATUS_LIGHT_WHITE, STATUS_LIGHT_GREEN};
        for(int c : colors) {
            controller.controlStatusLight(c);
            controller.stopStatusLight();
            sleep(1);
        }
        
        cout << "测试舱内灯" << endl;
        LEDcontroller.writeText("测试舱内灯", 0);
        controller.controlCabinLight(true);
        sleep(2);
        controller.controlCabinLight(false);

        cout << "测试行车灯" << endl;
        LEDcontroller.writeText("测试行车灯", 0);
        controller.controlDrivingLight(true);
        sleep(2);
        controller.controlDrivingLight(false);
        
        cout << "测试转向灯" << endl;
        LEDcontroller.writeText("测试转向灯", 0);
        controller.controlTurningLight(LEFT);
        sleep(3);
        controller.turnOffTurningLight();
        controller.controlTurningLight(RIGHT);
        sleep(3);
        controller.turnOffTurningLight();

        cout << "测试近光灯" << endl;
        LEDcontroller.writeText("测试近光灯", 0);
        controller.controlNearLight(true);
        sleep(2);
        controller.controlNearLight(false);

        cout << "测试远光灯" << endl;
        LEDcontroller.writeText("测试远光灯", 0);
        controller.controlFarLight(true);
        sleep(2);
        controller.controlFarLight(false);

        cout << "测试倒车灯" << endl;
        LEDcontroller.writeText("测试倒车灯", 0);
        controller.controlReversingLight(true);
        sleep(5);
        controller.controlReversingLight(false);

        cout << "测试刹车灯" << endl;
        LEDcontroller.writeText("测试刹车灯", 0);
        controller.controlBrakeLight(true);
        sleep(2);
        controller.controlBrakeLight(false);

        cout << "测试喇叭" << endl;
        LEDcontroller.writeText("测试喇叭", 0);
        controller.controlHorn(HORN_1);
        sleep(4);
        controller.stopHorn();
        controller.controlHorn(HORN_4);
        sleep(4);
        controller.stopHorn();

        cout << "测试传输" << endl;
        LEDcontroller.writeText("测试传输", 0);
        controller.controlDeliverToLeft(FRONT); // 前辊筒
        controller.controlDeliverToLeft(REAR); // 后辊筒
        sleep(3);
        controller.stopDeliver();
        controller.controlDeliverToRight(FRONT); // 前辊筒
        controller.controlDeliverToRight(REAR); // 后辊筒
        sleep(3);
        controller.stopDeliver();

        // // 仅辊筒转动
        // controller.RollingToRight(REAR); // 前辊筒
        // sleep(2);
        // controller.stopRolling();


        // cout << "测试卷帘门" << endl;
        // LEDcontroller.writeText("卷帘门测试", 0);
        // controller.downLeftRollingDoor();
        // sleep(35);
        // controller.riseLeftRollingDoor();
        // sleep(35);
        // controller.stopRollingDoor();
        // controller.downRightRollingDoor();
        // sleep(35);
        // controller.riseRightRollingDoor();
        // sleep(35);
        // controller.stopRollingDoor();

        // // 测试到位传感器
        // vector<bool> positionSensors;
        // while(1) {
        //     positionSensors = controller.readPositionSensor();
        //     for(auto i : positionSensors) {
        //         cout << i << ' ';
        //     }
        //     cout << endl;
        //     sleep(1);
        // }
    }
    LEDcontroller.initLED(); // 还原CRRC显示
    // adamPort2.disconnect(); // 断开连接
    return 0;
}