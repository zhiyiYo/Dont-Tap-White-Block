#include <iostream>
#include "BlockDetector.h"
#include "Screenshot.h"
#include "IOUtils.h"
using namespace std;
using cv::Mat;

int main(int argc, char const *argv[])
{
    BlockDetector detector;
    Screenshot capturer(1960, 1080);
    int x = 560;
    int y = 132;
    int width = 800;
    int height = 897;
    int pressedY = height * 5.0 / 8 + y;

    cout << "请在 2s 内切换到游戏界面！" << endl;
    Sleep(2000);

    while (1)
    {
        Mat screenshot = capturer.getScreenshot(x, y, width, height);
        int column = detector.findBlackBlock(screenshot);
        int pressedX = x + width * 1.0 / 8 * (column * 2 + 1);
        mouseEvent(pressedX, pressedY);
        cout << "按下第 " << column + 1 << " 列" << endl;
        Sleep(3000);
    }

    return 0;
}