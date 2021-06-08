#include <iostream>
#include "Serial.h"
#include <opencv2/opencv.hpp>
using namespace std;

int main(int argc, char const *argv[])
{
    Serial serial;

    // 初始化串口
    int port = argc == 1 ? 8 : atoi(argv[1]);
    if (!serial.initPort(port, 9600, 8, 1, 0))
        cout << "Open COM" << port << " wrong!" << endl;

    // 循环发送数据同时监听串口缓冲区
    int count = 0;
    while (1)
    {
        char msg = '0' + (++count) % 4;
        cout << "Send data: " << msg;
        cout << (serial.writeData(&msg, 1) ? " ✅" : " ❌") << endl;
        Sleep(1000);
    }

    return 0;
}