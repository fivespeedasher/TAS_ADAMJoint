#include <iostream>
#include <modbus.h>
#include <errno.h>
#include <vector>
#include <unistd.h>
#include <termios.h>
#include <fcntl.h>
// #include "hv/TcpClient.h"

using namespace std;
// using namespace hv;

int main() {
    const char* remote_host = "192.168.3.1"; // remote ip
    int port = 10125; // remote port
    // TcpClient cli;
    // int connfd = cli.createsocket(port, remote_host); // 连接套接字
    // if (connfd < 0) {
    //     return -1;
    // }

    // 创建Modbus TCP上下文
    modbus_t *ctx = modbus_new_tcp("192.168.3.1", 10125);
    if (ctx == NULL) {
        cerr << "Unable to create the libmodbus context" << endl;
        return -1;
    }

    // 设置应答延时1s
    modbus_set_response_timeout(ctx, 0, 1000000);

    // 启用调试模式
    modbus_set_debug(ctx, false);

    modbus_set_slave(ctx, 4);

    // 连接到Modbus服务器
    if (modbus_connect(ctx) == -1) {
        cerr << "Connection failed: " << modbus_strerror(errno) << endl;
        modbus_free(ctx);
        modbus_close(ctx);
        return -1;
    }

    // // 创建TCP客户端
    // TcpClient client;
    // cli.onConnection = [&](const SocketChannelPtr& channel) {
    //     if (channel->isConnected()) {
    //         cout << "Connected to server!" << endl;

            // 读取线圈状态
            vector<uint8_t> coils(16);
            while(1) {
                modbus_close(ctx);

                modbus_set_slave(ctx, 4);

                // 连接到Modbus服务器
                if (modbus_connect(ctx) == -1) {
                    cerr << "Connection failed: " << modbus_strerror(errno) << endl;
                    modbus_free(ctx);
                    modbus_close(ctx);
                    return -1;
                }

                sleep(1);
                if(modbus_read_bits(ctx, 0, 16, coils.data()) == -1) {
                    cout << "Failed to read coils: " << modbus_strerror(errno) << endl;
                    modbus_close(ctx);
                    modbus_free(ctx);
                    return -1;
                }
                cout << "Coils status: ";
                for(auto coil: coils) {
                    cout << static_cast<int>(coil) << " ";
                }
                cout << endl << endl;
            }
    //     } else {
    //         cerr << "Failed to connect to server!" << endl;
    //     }
    // };

    // cli.onMessage = [&](const SocketChannelPtr& channel, Buffer* buf) {
    //     // 处理服务器响应
    //     cout << "Received message from server: " << buf->data() << endl;
    // };
    // // 设置服务器地址和端口
    // cli.start();

    // // 运行事件循环
    // hloop_run(cli.loop());

    return 0;
}