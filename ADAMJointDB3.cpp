#include "include/ADAM.h"
#include "include/BulbConstants.h"
#include <iostream>
#include <vector>
#include <termios.h> //TCSANOW
#include <string.h>
#include <fcntl.h> // fcntl

using namespace std;

int main() {
    const char* remote_host = "192.168.3.1"; // remote ip
    int port = 10127; // remote port 3

    // 基类ADAM: 放置串口连接的参数
    ADAM adamPort2(remote_host, port);
    adamPort2.connect(false); // 不调试状态下连接
    // 初始化通道
    int slave_ID_1 = 1; // 从机地址
    int totalCH_1 = 8; // 从机通道总数
    int slave_ID_2 = 2; // 从机地址
    int totalCH_2 = 8; // 从机通道总数
    int slave_ID_3 = 3; // 从机地址
    int totalCH_3 = 8; // 从机通道总数

    ADAM4068 adam_1(adamPort2, slave_ID_1, totalCH_1);
    ADAM4068 adam_2(adamPort2, slave_ID_2, totalCH_2);
    ADAM4068 adam_3(adamPort2, slave_ID_3, totalCH_3);
    
    bool RUN_status = true; // 行车灯状态

    while(true) {
        int current_slave;
        int current_ch;
        bool current_flag;

        cout << "请输入控制的从机号: " << endl;
        cin >> current_slave;
        cout << "请输入通道和状态: " << endl;
        cin >> current_ch >> current_flag;

        switch (current_slave) {
            case 1:
                adam_1.write_coil(current_ch, current_flag);
                break;
            case 2:
                adam_2.write_coil(current_ch, current_flag);
                break;
            case 3:
                adam_3.write_coil(current_ch, current_flag);
                break;
            default:
                cout << "无效的从机号" << endl;
                break;
        }
        cout << "已成功设置" << endl;
    }
    adamPort2.disconnect();
    return 0;
}