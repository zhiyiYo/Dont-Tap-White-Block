#include "Serial.h"
#include "IOUtils.h"
#include "BlockDetector.h"

int main(int argc, char const *argv[])
{
    // 打开 USB 摄像头
    cv::VideoCapture camera(1);
    cv::Mat cameraImage, gameImage;

    // 创建黑块探测器实例
    BlockDetector detector;

    // 创建串口实例
    Serial serial;

    // 设置游戏区域
    cv::Rect gameRegion(150, 25, 320, 430);

    bool isPlaying = false;
    bool endGame = false;
    std::cout << "🐛 按下 `S` 开始比赛\n🦄 按下 `Q` 结束运行" << std::endl;

    // 读入摄像头的拍摄内容
    while (!endGame)
    {
        camera >> cameraImage;
        gameImage = cameraImage(gameRegion);
        cv::rectangle(cameraImage, gameRegion, cv::Scalar(0, 255, 0), 2);

        // 检测按键
        char key = getPressedKey();
        switch (key)
        {
        case 's':
            isPlaying = true;
            std::cout << "🚀 开始游戏！" << std::endl;
            break;
        case 'q':
            endGame = true;
            std::cout << "🎯 游戏结束！" << std::endl;
            break;
        default:
            break;
        }

        // 游戏开始
        if (isPlaying)
        {
            // 寻找黑块
            int column = detector.findBlackBlock(gameImage, 70, 9000, cv::Size(3, 3));
            if (column >= 0)
            {
                // 绘制黑块轮廓线
                cv::rectangle(gameImage, detector.getBlockRect(), cv::Scalar(0, 0, 255), 2);
            }
        }

        // 显示图像
        imshow("camera", cameraImage);
        cv::waitKey(30);
    }

    return 0;
}