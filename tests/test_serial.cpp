#include <iostream>
#include "Serial.h"
using namespace std;

int main()
{
    char a[] = "0ABC";
    Serial mySerial;

    // 初始化串口
    if (!mySerial.initPort(5, 9600, 8, 1, 0))
    {
        cout << "Open COM wrong!" << endl;
    }

    // 创建监听串口的线程
    if (!mySerial.openThread())
    {
        cout << "Open thread wrong!";
    }

    // 向串口发送数据
    cout << "Send data: " << (mySerial.writeData(a, 4) ? "Successful" : "Fail") << endl;

    while (1)
    {
        // pass
    }

    return 0;
}