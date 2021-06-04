#include <iostream>
#include "Serial.h"
using namespace std;

int main(int argc, char const *argv[])
{
    Serial s0, s1;

    // 初始化串口
    int port_0 = argc == 1 ? 8 : argv[1][0] - '0';
    int port_1 = argc <= 2 ? 9 : argv[2][0] - '0';
    if (!s0.initPort(port_0, 9600, 8, 1, 0))
        cout << "Open COM" << port_0 << " wrong!" << endl;
    if (!s0.openThread())
        cout << "s0 open listening thread failed";
    if (!s1.initPort(port_1, 9600, 8, 1, 0))
        cout << "Open COM" << port_1 << " wrong!" << endl;
    if (!s1.openThread())
        cout << "s1 open listening thread failed";

    int count = 0;
    char msg = '0';

    // 循环发送数据
    while (1)
    {
        // 循环监听端口
        msg += (++count) % 4;
        cout << "Send data: " << msg;
        if (msg == '0' || msg == '1')
            cout << (s0.writeData(&msg, 1) ? " ✅" : " ❌") << endl;
        else
            cout << (s1.writeData(&msg, 1) ? " ✅" : " ❌") << endl;
        Sleep(500);
        msg = '0';
    }

    return 0;
}