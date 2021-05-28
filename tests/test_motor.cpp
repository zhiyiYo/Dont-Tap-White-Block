#include <iostream>
#include "Serial.h"
using namespace std;

int main(int argc, char const *argv[])
{
    Serial serial;
    // 串口号
    int port = argc == 1 ? 5 : argv[1][0] - '0';
    char msg = '1';

    // 初始化串口
    if (!serial.initPort(port, 9600, 8, 1, 0))
    {
        cout << "Open COM" << port << " failed!" << endl;
    }

    while (1)
    {
        cout << "Send data: " << msg
             << (serial.writeData(&msg, 1) ? " ✅" : " ❌") << endl;
        Sleep(1000);
    }
    return 0;
}