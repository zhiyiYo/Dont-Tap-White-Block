#include <iostream>
#include "BlockDetector.h"
#include "Screenshot.h"
#include "IOUtils.h"
using namespace std;
using namespace cv;

int main(int argc, char const *argv[])
{
    BlockDetector detector;
    Screenshot capturer;

    // 屏幕缩放比
    double zoom = capturer.getZoom();

    // 截屏区域
    int x = 1041;
    int y = 132;
    int width = 800;
    int height = 880;

    int nPress = 0;
    int pressedY = (height * 6.0 / 8 + y) / zoom;

    cout << "🐛 按下 `S` 开始比赛！\n🦄 按下 `Q` 结束运行" << endl;

    while (getPressedKey() != 's')
    {
        // pass
    }

    cout << "开始游戏！" << endl;

    while (getPressedKey() != 'q')
    {
        Mat screenshot = capturer.getScreenshot(x, y, width, height);
        int column = detector.findBlackBlock(screenshot);
        Rect blockRect = detector.getBlockRect();

        // 当黑块的下边缘大于
        int blackBottom = blockRect.height + y + blockRect.y;
        if (column >= 0 && blackBottom >= pressedY)
        {
            int pressedX = (x + width * 1.0 / 8 * (column * 2 + 1)) / zoom;
            mouseClick(pressedX, pressedY);
            printf("[ %2d ] 按下第 %d 列\n", ++nPress, column + 1);
        }
    }

    return 0;
}