#include "hv/TcpServer.h"
using namespace hv;

int main() {
    int port = 12345;
    TcpServer srv;
    int listenfd = srv.createsocket(port); // 监听套接字
    if (listenfd < 0) {
        return -1;
    }
    printf("server listen on port %d, listenfd=%d ...\n", port, listenfd);
    srv.onConnection = [](const SocketChannelPtr& channel) {
        std::string peeraddr = channel->peeraddr(); // 远程客户端的地址 ip:port
        if (channel->isConnected()) {
            printf("%s connected! connfd=%d\n", peeraddr.c_str(), channel->fd());
        } else {
            printf("%s disconnected! connfd=%d\n", peeraddr.c_str(), channel->fd());
        }
    };
    srv.onMessage = [](const SocketChannelPtr& channel, Buffer* buf) {
        // echo
        channel->write(buf); // 将接收到的数据原样返回给当前客户端
        printf("< %.*s\n", (int)buf->size(), (char*)buf->data());
    };
    srv.setThreadNum(4); // 设置线程数
    srv.start();

    // press Enter to stop
    while (getchar() != '\n');
    return 0;
}