#include "LEDScreenController.h"
#include <fcntl.h>
#include <string.h>
// #include <errno.h>
// #include "utils.h" // 不依赖utils.h，而是使用前置声明，可以在修改utils.h后不用重新编译LEDScreenController.cpp

using namespace std;
std::vector<uint16_t> gbk_hex(const std::string& gbk_str);

/**
 * @brief Construct a new LEDScreenController::LEDScreenController object
 * 
 * @param ip 
 * @param port 
 * @param setDebug 是否启动调试模式
 */
LEDScreenController::LEDScreenController(const char *ip, int port, bool setDebug)
     : ip(ip), port(port), setDebug(setDebug) {
    // param
    this->save_state = 0;    // 0: 不保存状态，1: 保存状态
    this->brightness = 240; // 亮度
    this->page = 0;      // 操作的页面
    this->zone = 0;     // 操作的分区
    this->align = 1;    // 0: 左对齐，1: 居中，2: 右对齐
    this->size = 12;     // 字体大小
    this->speaker = 0;  // 0为不发声，1为发声
    this->color = 71;   // 字体，71为红色
}

LEDScreenController::~LEDScreenController() {
    disconnect();
}

int LEDScreenController::set_non_blocking(int fd) {
    int flags = fcntl(fd, F_GETFL, 0);
    if (flags == -1) {
        cerr << "Error getting flags: " << strerror(errno) << endl;
        return -1;
    }

    if (fcntl(fd, F_SETFL, flags | O_NONBLOCK) == -1) {
        cerr << "Error setting non-blocking mode: " << strerror(errno) << endl;
        return -1;
    }
    return 0;
}

// 使用modbus-tcp
bool LEDScreenController::connect() {
    // 打开端口
    this->ctx = modbus_new_tcp(this->ip, this->port);

    if(this->ctx == nullptr) {
        cout << "Unable to create the libmodbus context: " << modbus_strerror(errno) << endl;
        return -1;
    }
    // 设置从机地址
    modbus_set_slave(this->ctx, 1);
    // connect
    if(modbus_connect(this->ctx) == -1) {
        cout << "Connection to slave failed: " << modbus_strerror(errno) << endl;
        return -1;
    }
    // 设置应答延时
    modbus_set_response_timeout(this->ctx, 0, 1000000);

    // 启用调试模式
    if(setDebug) {
        modbus_set_debug(this->ctx, setDebug);
    }
    
    // connect
    if(modbus_connect(this->ctx) == -1) {
        cerr << "Connection to slave failed: " << modbus_strerror(errno) << endl;
        modbus_free(this->ctx);
        modbus_close(this->ctx);
        return -1;
    }
    sleep(1);
    
    // 获取底层的套接字文件描述符
    int sockfd = modbus_get_socket(ctx);
    if (set_non_blocking(sockfd) == -1) {
        modbus_free(ctx);
        return -1;
    }

    // 等待连接完成
    fd_set writefds;
    struct timeval tv;
    tv.tv_sec = 2;  // 2秒超时
    tv.tv_usec = 0;
    FD_ZERO(&writefds);
    FD_SET(sockfd, &writefds);

    int retval = select(sockfd + 1, NULL, &writefds, NULL, &tv);
    if (retval == -1) {
        cerr << "Select error: " << strerror(errno) << endl;
        modbus_free(ctx);
        return -1;
    } else if (retval == 0) {
        cerr << "Connection timeout" << endl;
        modbus_free(ctx);
        return -1;
    }
    this->ctx = ctx;
    return true;
}

void LEDScreenController::disconnect() {
    if (ctx) {
        modbus_close(ctx);
        modbus_free(ctx);
    }
}

bool LEDScreenController::turnOn() {
    vector<uint16_t> screen(1, 1); // 0为关闭，其他为打开
    if(modbus_write_registers(ctx, 4220, 1, screen.data()) == -1) {
        cout << "Write register failed: " << modbus_strerror(errno) << endl;
        return false;
    }
    else {cout << "Successfully turn on" << endl; }
    return true;
}

bool LEDScreenController::turnOff() {
    vector<uint16_t> screen(1, 0); // 0为关闭，其他为打开
    if(modbus_write_registers(ctx, 4220, 1, screen.data()) == -1) {
        cout << "Write register failed: " << modbus_strerror(errno) << endl;
        return false;
    }
    else {cout << "Successfully turn off" << endl;}
    return true;
}

bool LEDScreenController::setScreen(uint16_t save_state, uint16_t brightness, uint16_t page) {
    vector<uint16_t> screen_param = {save_state, 0, brightness, 0, 0, page};
    if(modbus_write_registers(ctx, 4002, 6, screen_param.data()) == -1) {
        cout << "Write register failed: " << modbus_strerror(errno) << endl;
        return false;
    }
    else {cout << "Successfully set screen" << endl;}
    return true;
}

/**
 * @brief 写入text到屏幕
 * 
 * @param text 文本（超过8字就滚动显示）
 * @param save_state 重新上电时0: 不保存状态，1: 保存状态
 * @return true 
 * @return false 
 */
bool LEDScreenController::writeText(const string& text, uint16_t save_state) {
    vector<uint16_t> text_hex;
    // text的GBK码
    text_hex = gbk_hex(text);

    vector<uint16_t> text_param = {this->page, this->zone, save_state, this->align, this->size, this->color, this->speaker};
    // 尾插text的GBK码
    text_param.insert(text_param.end(), text_hex.begin(), text_hex.end());
    if(modbus_write_registers(ctx, 4018, text_param.size(), text_param.data()) == -1) {
        cout << "Write register failed: " << modbus_strerror(errno) << endl;
    }
    else {cout << "Successfully write text" << endl; }
    return true;
}

// 初始化LED，显示：CRRC
int LEDScreenController::initLED() {
    LEDScreenController::turnOn();
    LEDScreenController::setScreen(save_state, brightness, page);
    string text = {"CRRC"};
    save_state = 1;
    LEDScreenController::writeText(text, save_state);
    return 0;
}