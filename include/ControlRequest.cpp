// 以DB2接入六个ADAM模块，以DB1接显示屏
using namespace std;
ControlRequest::ControlRequest() {}
ControlRequest::~ControlRequest() {}
int ControlRequest::connect(const char *ip, int port) {
    ADAM adam(ip, port);
    if (adam.connect(false) != 0) {
        return -1;
    }
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

    ADAM4068 adam_1(adamPort2, slave_ID_1, totalCH_1);
    ADAM4068 adam_2(adamPort2, slave_ID_2, totalCH_2);
    ADAM4068 adam_3(adamPort2, slave_ID_3, totalCH_3);
    ADAM4051 adam_4(adamPort1, slave_ID_4, totalDI_4);
    ADAM4051 adam_5(adamPort1, slave_ID_5, totalDI_5);
    ADAM4168 adam_6(adamPort1, slave_ID_6, totalCH_6, duty_cycles);

    //TODO 初始化显示屏
    return 0;
}

// 打开行车灯
void ControlRequest::controlDrivingLight(bool state) {
    adam_6.SetDO(RUNNING_LIGHT, state); 
}

// 打开转向灯
void ControlRequest::controlLeftTurningLight(bool state) {
    adam_6.SetDO(TURNING, state); 
}

// 打开倒车灯
void ControlRequest::controlReversingLight(bool state) {
    adam_6.SetDO(MOVEBACK, state); // 打开倒车灯
}