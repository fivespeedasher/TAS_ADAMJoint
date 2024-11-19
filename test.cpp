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
    ControlRequest controller(remote_host, port);
    controller.connect();
    controller.controlWarningLight(true);
    return 0;
}