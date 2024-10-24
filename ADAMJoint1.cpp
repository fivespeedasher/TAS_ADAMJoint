#include "ADAM.h"
#include "BulbConstants.h"
#include <iostream>
#include <vector>
#include <termios.h> //TCSANOW
#include <string.h>
#include <fcntl.h> // fcntl
#include <hv/TcpClient.h>

using namespace std;

bool kbhit() {
    termios oldt, newt;
    int ch;
    int oldf;

    tcgetattr(STDIN_FILENO, &oldt);
    newt = oldt;
    newt.c_lflag &= ~(ICANON | ECHO);
    tcsetattr(STDIN_FILENO, TCSANOW, &newt);
    oldf = fcntl(STDIN_FILENO, F_GETFL, 0);
    fcntl(STDIN_FILENO, F_SETFL, oldf | O_NONBLOCK);

    ch = getchar();

    tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
    fcntl(STDIN_FILENO, F_SETFL, oldf);

    if(ch != EOF) {
        ungetc(ch, stdin);
        return true;
    }

    return false;
}

int main() {
    const char* remote_host = "192.168.3.1"; // remote ip
    int port = 10125; // remote port

    // 基类ADAM: 放置串口连接的参数
    ADAM adamPort1(remote_host, port);

    int slave_ID_4 = 4; // 从机地址
    int slave_ID_5 = 5;
    int slave_ID_6 = 6;
    int totalDI_4 = 16; // 从机DI总数
    int totalDI_5 = 16;
    int totalCH_6 = 8; // 从机通道总数
    int duty_cycles = 0.5; // 脉冲占空比
    ADAM4051 adam_4(adamPort1, slave_ID_4, totalDI_4);
    ADAM4051 adam_5(adamPort1, slave_ID_5, totalDI_5);
    // ADAM4168 adam_6(adamPort1, slave_ID_6, totalCH_6, duty_cycles);
    
    // bool RUN_status = true; // 行车灯状态

    // adam_6.SetDO(RUNNING_LIGHT, RUN_status); // 打开行车灯
    while(true) {
        // sleep(1);
        if(adam_4.read_coils() != -1) {
            vector<uint8_t> state_coils_4 = adam_4.state_coils;
            cout << "ADAM-4051(4): ";
            // 4051COM悬空时，DI=1表示接收到了低电平
            for(auto coil: state_coils_4) {
                cout << static_cast<int>(coil) << " ";
            }
            cout << endl;
            // if(state_coils_4[0] == 1) {
            //     // 启动左转向灯
            //     vector<int> PulseChannel = {TURNING}; // 脉冲通道
            //     adam_6.StartPulse(PulseChannel, BLINK); // 打开转向
            // }
        }
        if(adam_5.read_coils() != -1) {
            vector<uint8_t> state_coils_5 = adam_5.state_coils;
            cout << "ADAM-4051(5): ";
            for(auto coil: state_coils_5) {
                cout << static_cast<int>(coil) << " ";
            }
            cout << endl;
        //     if(state_coils_5[0] == 1) {
        //         RUN_status = !RUN_status;
        //         adam_6.SetDO(RUNNING_LIGHT, RUN_status); // 打开行车灯
        //     }
        }

        // 检查是否有按键按下
        if (kbhit()) {
            cout << "Key pressed, exiting..." << endl;
            adamPort1.disconnect();
            break;
        }
    }
    // adamPort1.disconnect();
    return 0;
}