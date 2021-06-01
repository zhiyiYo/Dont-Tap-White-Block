#include "Serial.h"
#include "BlockDetector.h"
#define WINDOW_NAME "camera"

void on_playChanged(int isPlaying, void *userData);

int g_isPlaying = 0;
int g_isEndGame = 0;

int main(int argc, char const *argv[])
{
    // 打开 USB 摄像头
    cv::VideoCapture camera(1);
    cv::Mat cameraImage, gameImage;

    // 创建黑块探测器实例
    BlockDetector detector;

    // 创建串口实例
    Serial serial;
    if (!serial.initPort(8, 9600, 8, 1, 0))
    {
        std::cerr << "打开串口失败！" << std::endl;
        return -1;
    };

    // 设置游戏区域
    cv::Rect gameRegion(150, 312, 320, 430 / 3);

    // 创建图窗
    cv::namedWindow(WINDOW_NAME);
    cv::createTrackbar("Play game", WINDOW_NAME, &g_isPlaying, 1, on_playChanged, NULL);

    // 敲击是否结束标志位
    bool isClickDone = true;

    // 读入摄像头的拍摄内容
    while (!g_isEndGame)
    {
        camera >> cameraImage;
        gameImage = cameraImage(gameRegion);
        cv::rectangle(cameraImage, gameRegion, cv::Scalar(0, 255, 0), 2);

        // 游戏开始
        if (g_isPlaying)
        {
            // 寻找黑块
            int column = detector.findBlackBlock(gameImage, 70, 6000, cv::Size(2, 2));
            // 绘制黑块轮廓线
            cv::rectangle(gameImage, detector.getBlockRect(), cv::Scalar(0, 0, 255), 2);
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
        imshow(WINDOW_NAME, cameraImage);
        cv::waitKey(30);
    }

    return 0;
}

void on_playChanged(int isPlaying, void *userData)
{
    std::string info = isPlaying ? "⏸  游戏开始" : "⏺  游戏暂停";
    std::cout << info << std::endl;
}