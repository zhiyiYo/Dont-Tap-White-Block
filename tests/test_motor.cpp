#include <iostream>
#include "Serial.h"
using namespace std;

int main(int argc, char const *argv[])
{
    char msg = '1';
    Serial serial;

    // 初始化串口
    if (!serial.initPort(5, 9600, 8, 1, 0))
    {
        cout << "Open COM wrong!" << endl;
    }

    while (1)
    {
        cout << "Send data: " << msg
             << (serial.writeData(&msg, 1) ? " ✅" : " ❌") << endl;
        Sleep(1000);
    }
    return 0;
}