#include <iostream>
#include "hv/TcpClient.h"
#include "ADAM.h"
#include <unistd.h> // sleep
#include "BulbConstants.h"
#include "hv/htime.h"
#include <thread>
using namespace hv;

int run_loop() {
    const char* remote_host = "192.168.3.1"; // remote ip
    int port = 10125; // remote port
    ADAM adamsPort2(remote_host, port);

    int slave_ID_4 = 4; // 从机地址
    int slave_ID_5 = 5;
    int slave_ID_6 = 6;
    int totalDI_4 = 16; // 从机DI总数
    int totalDI_5 = 16;
    int totalCH_6 = 8; // 从机通道总数
    int duty_cycles = 0.5; // 脉冲占空比
    ADAM4051 adam_4(adamsPort2, slave_ID_4);
    ADAM4051 adam_5(adamsPort2, slave_ID_5);
    // ADAM4168 adam_6(adamsPort2, slave_ID_6, totalCH_6, duty_cycles);

    if(adam_4.read_coils() != -1) {
        vector<uint8_t> state_coils_4 = adam_4.state_coils;
        cout << "ADAM-4051(4): ";
        // 4051COM悬空时，DI=1表示接收到了低电平
        for(auto coil: state_coils_4) {
            cout << static_cast<int>(coil) << " ";
        }
        cout << endl;
    }
    return 0;
}

int main() {
    static const char* remote_host = "192.168.3.1"; // remote ip
    static int port = 10125; // remote port

    TcpClient cli;
    int connfd = cli.createsocket(port, remote_host); // 连接套接字
    if (connfd < 0) {
        return -20;
    }
    cli.onConnection = [](const SocketChannelPtr& channel) {
        std::string peeraddr = channel->peeraddr();
        if (channel->isConnected()) {
            printf("connected to %s! connfd=%d\n", peeraddr.c_str(), channel->fd());
            // send(time) every 3s
            setInterval(2000, [channel](TimerID timerID) {  
                if (channel->isConnected()) {
                    char str[DATETIME_FMT_BUFLEN] = {0};
                    datetime_t dt = datetime_now();
                    datetime_fmt(&dt, str);
                    thread run_thread(run_loop);
                    run_thread.detach();
                } else {
                    killTimer(timerID);
                }
            });
        }else {
            printf("disconnected to %s! connfd=%d\n", peeraddr.c_str(), channel->fd());
        }
    };
    cli.onMessage = [](const SocketChannelPtr& channel, Buffer* buf) {
        printf("< %.*s\n", (int)buf->size(), (char*)buf->data());
    };

       // reconnect: 1,2,4,8,10,10,10...
    reconn_setting_t reconn;
    reconn_setting_init(&reconn);
    reconn.min_delay = 1000;
    reconn.max_delay = 10000;
    reconn.delay_policy = 2;
    cli.setReconnect(&reconn);
    cli.start();

    // press Enter to stop
    while (getchar() != '\n');


    // // main loop
    // bool RUN_status = true; // 行车灯状态
    // adam_6.SetDO(RUNNING_LIGHT, RUN_status); // 打开行车灯
    // while (true) {
        // if(adam_4.read_coils() != -1) {
        //     vector<uint8_t> state_coils_4 = adam_4.state_coils;
        //     if(state_coils_4[0] == 1) {
        //         // 启动左转向灯
        //         vector<int> PulseChannel = {TURNING}; // 脉冲通道
        //         adam_6.StartPulse(PulseChannel, BLINK); // 打开转向
        //     }
        // }
        // if(adam_5.read_coils() != -1) {
        //     vector<uint8_t> state_coils_5 = adam_5.state_coils;
        //     if(state_coils_5[0] == 1) {
        //         RUN_status = !RUN_status;
        //         adam_6.SetDO(RUNNING_LIGHT, RUN_status); // 打开行车灯
        //     }
        // }


        // if(adam_5.read_coils() != -1) {
        //     vector<uint8_t> state_coils_5 = adam_5.state_coils;
        //     cout << "ADAM-4051(5): ";
        //     for(auto coil: state_coils_5) {
        //         cout << static_cast<int>(coil) << " ";
        //     }
        //     cout << endl;
        // }
    //     sleep(1);
    // }
    return 0;
}