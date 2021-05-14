#include <iostream>
#include "Serial.h"
using namespace std;

int main()
{
    char *a = new char[4];
    SerialCom mySerial;

    if (!mySerial.initPort(3, 9600, 8, 1, 0))
    {
        cout << "打开串口错误" << endl;
    }
    if (!mySerial.openThread())
    {
        cout << "创建线程错误";
    }
    a[0] = '1';
    a[1] = 'A';
    a[2] = 'B';
    a[3] = 'C';
    cout << "发送数据：" << mySerial.writeData(a, 4) << endl;
    system("pause");
    return 0;
}