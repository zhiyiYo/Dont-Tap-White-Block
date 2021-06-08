#include <iostream>
#include "BlockDetector.h"
#include "Screenshot.h"
#include "IOUtils.h"

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

    std::cout << "🐛 按下 `S` 开始比赛！\n🦄 按下 `Q` 结束运行" << std::endl;

    while (getPressedKey() != 's')
    {
        // pass
    }

    std::cout << "开始游戏！" << std::endl;

    while (getPressedKey() != 'q')
    {
        // 截屏并检测黑块
        cv::Mat screenshot = capturer.getScreenshot(x, y, width, height);
        int column = detector.findBlackBlock(screenshot, 70);
        cv::Rect blockRect = detector.getBlockRect();

        // 当黑块的下边缘大于 pressedY 时触发点击
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