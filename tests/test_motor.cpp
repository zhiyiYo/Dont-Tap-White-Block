#include <iostream>
#include <string>
#include "BlockDetector.h"
#include "Serial.h"
using namespace std;
using namespace cv;

int main(int argc, char const *argv[])
{
    char msg = '0';
    BlockDetector detector;
    Serial serial;
    Mat image = imread("resource/images/1.png");

    // 初始化串口
    if (!serial.initPort(5, 9600, 8, 1, 0))
    {
        cout << "Open COM wrong!" << endl;
    }

    while (1)
    {
        msg = detector.findBlackBlock(image) + '0';
        cout << "Send data: " << msg
             << (serial.writeData(&msg, 1) ? " ✅" : " ❌") << endl;
        Sleep(1000);
    }
    return 0;
}