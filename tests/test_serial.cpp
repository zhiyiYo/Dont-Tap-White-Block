#include <iostream>
#include "Serial.h"
using namespace std;

int main()
{
    char a[] = "1ABC";
    Serial serial;

    // 初始化串口
    if (!serial.initPort(5, 9600, 8, 1, 0))
    {
        cout << "Open COM wrong!" << endl;
    }

    // 创建监听串口的线程
    if (!serial.openThread())
    {
        cout << "Open thread wrong!";
    }

    // 向串口发送数据
    cout << "Send data: " << a << (serial.writeData(a, 4) ? " ✅" : " ❌") << endl;

    while (1)
    {
        // pass
    }

    return 0;
}