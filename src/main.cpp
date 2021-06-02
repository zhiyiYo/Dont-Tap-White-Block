#include "Serial.h"
#include "BlockDetector.h"
#include "ScreenDetector.h"
#define CAMERA_WINDOW "camera"
#define GAME_WINDOW "game"

void on_playChanged(int isPlaying, void *userData);

int g_isPlaying = 0;
int g_isEndGame = 0;

int main(int argc, char const *argv[])
{
    // 打开 USB 摄像头
    cv::VideoCapture camera(1);
    cv::Mat cameraImage, gameImage, screenImage;

    // 创建黑块探测器和手机屏幕探测器
    BlockDetector blockDetector;
    ScreenDetector screenDetector;

    // 创建串口实例
    Serial serial;
    if (!serial.initPort(8, 9600, 8, 1, 0))
    {
        std::cerr << "打开串口失败！" << std::endl;
        return -1;
    };

    // 创建图窗
    cv::namedWindow(CAMERA_WINDOW);
    cv::namedWindow(GAME_WINDOW);
    cv::createTrackbar("Play game", CAMERA_WINDOW, &g_isPlaying, 1, on_playChanged, NULL);

    // 敲击是否结束标志位
    bool isClickDone = true;

    // 读入摄像头的拍摄内容
    while (!g_isEndGame)
    {
        camera >> cameraImage;

        // 检测手机屏幕并截取出探测区域
        cv::Size kernelSize(7, 7);
        screenImage = screenDetector.getScreen(cameraImage, 125, 5000, kernelSize);
        cv::Rect gameRegion((kernelSize.width + 1) / 2,
                            screenImage.rows / 2,
                            screenImage.cols - (kernelSize.width + 3),
                            screenImage.rows / 4);
        cameraImage = screenDetector.drawScreenRect(cameraImage); // 绘制手机轮廓
        gameImage = screenImage(gameRegion);
        cv::rectangle(screenImage, gameRegion, cv::Scalar(0, 255, 0), 2);

        // 游戏开始
        if (g_isPlaying)
        {
            // 寻找黑块
            // std::cout << gameRegion.area()<<std::endl;
            int column = blockDetector.findBlackBlock(gameImage, 180, gameRegion.area() / 8, cv::Size(2, 2));
            // 绘制黑块轮廓线
            cv::rectangle(gameImage, blockDetector.getBlockRect(), cv::Scalar(0, 0, 255), 2);
            if ((column == 3 || column == 2) && isClickDone)
            {
                // 发送敲击命令
                char col = '0' + column;
                serial.writeData(&col, 1);
                // 在敲击完成前禁止继续敲击
                isClickDone = false;
            }

            // 检查敲击是否完成
            std::string msg = serial.readData();
            if (msg == "d")
                isClickDone = true;
        }

        // 显示图像
        cv::imshow(CAMERA_WINDOW, cameraImage);
        cv::imshow(GAME_WINDOW, screenImage/* blockDetector.drawBlackBlock() */);
        cv::waitKey(30);
    }

    return 0;
}

void on_playChanged(int isPlaying, void *userData)
{
    std::string info = isPlaying ? "⏸  游戏开始" : "⏺  游戏暂停";
    std::cout << info << std::endl;
}