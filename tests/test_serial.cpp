#include <iostream>
#include "Serial.h"
using namespace std;

int main(int argc, char const *argv[])
{
    Serial serial;

    // 初始化串口
    int port = argc == 1 ? 5 : argv[1][0] - '0';
    if (!serial.initPort(port, 9600, 8, 1, 0))
    {
        cout << "Open COM wrong!" << endl;
    }

    int count = 0;
    char msg = '1';

    // 向串口发送数据
    cout << "Send data: " << msg << (serial.writeData(&msg, 1) ? " ✅" : " ❌") << endl;

    // 创建监听串口的线程
    if (serial.openThread())
    {
        while (1)
        {
            // 循环监听端口
            /* msg += (++count) % 4; */
            cout << "Send data: " << msg << (serial.writeData(&msg, 1) ? " ✅" : " ❌") << endl;
            Sleep(1000);
            //msg = '0';
        }
    }
    else
        cout << "Open thread wrong!";

    return 0;
}