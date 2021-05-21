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

    // 向串口发送数据
    cout << "Send data: " << a << (serial.writeData(a, 4) ? " ✅" : " ❌") << endl;

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