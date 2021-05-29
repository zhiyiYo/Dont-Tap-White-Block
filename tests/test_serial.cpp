#include <iostream>
#include "Serial.h"
using namespace std;

int main(int argc, char const *argv[])
{
    Serial serial;
    char msg[] = "1";
    int port = argc == 1 ? 5 : argv[1][0] - '0';

    // 初始化串口
    if (!serial.initPort(port, 9600, 8, 1, 0))
    {
        cout << "Open COM wrong!" << endl;
    }

    // 向串口发送数据
    cout << "Send data: " << msg << (serial.writeData(msg, 1) ? " ✅" : " ❌") << endl;

    // 创建监听串口的线程
    if (serial.openThread())
        while (1)
        {
            // 循环监听端口
        }
    else
        cout << "Open thread wrong!";

    return 0;
}