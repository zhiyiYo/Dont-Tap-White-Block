#include "Serial.h"
#include "BlockDetector.h"
#include "ScreenDetector.h"
#define CAMERA_WINDOW "camera"
#define GAME_WINDOW "game"

void on_playChanged(int isPlaying, void *userData);
void updateClickState(Serial &s0, Serial &s1, bool &isClickDone);

int main(int argc, char const *argv[])
{
    // 标志位
    int isPlaying = 0;
    int isEndGame = 0;

    // 打开 USB 摄像头
    cv::VideoCapture camera(1);
    cv::Mat cameraImage, gameImage, screenImage;

    // 创建黑块探测器和手机屏幕探测器
    BlockDetector blockDetector;
    ScreenDetector screenDetector;

    // 创建串口实例
    Serial serial_0, serial_1;
    if (!serial_0.initPort(8, 9600, 8, 1, 0))
    {
        std::cerr << "打开串口 COM8 失败！" << std::endl;
        return -1;
    };
    if (!serial_1.initPort(9, 9600, 8, 1, 0))
    {
        std::cerr << "打开串口 COM9 失败！" << std::endl;
        return -1;
    };

    // 创建图窗
    cv::namedWindow(CAMERA_WINDOW);
    cv::namedWindow(GAME_WINDOW);
    cv::createTrackbar("Play game", CAMERA_WINDOW, &isPlaying, 1, on_playChanged, NULL);

    // 敲击是否结束标志位
    bool isClickDone = true;
    // bool isClickDones[4] = {true, true, true, true};

    // 读入摄像头的拍摄内容
    while (!isEndGame)
    {
        camera >> cameraImage;

        // 检测手机屏幕
        cv::Size kernelSize(9, 9);
        screenImage = screenDetector.getScreen(cameraImage, 125, 5000, kernelSize);
        cameraImage = screenDetector.drawScreenRect(cameraImage); // 绘制手机轮廓

        // 截取游戏区域
        cv::Rect gameRegion((kernelSize.width + 1) / 2,
                            screenImage.rows / 2,
                            screenImage.cols - (kernelSize.width + 1),
                            (screenImage.rows - kernelSize.height - 1) / 4);
        cv::rectangle(screenImage, gameRegion, cv::Scalar(0, 255, 0), 2);
        gameImage = screenImage(gameRegion);

        // 游戏开始
        if (isPlaying)
        {
            // 寻找黑块
            int column = blockDetector.findBlackBlock(gameImage, 180, gameRegion.area() / 8, cv::Size(2, 2));
            // 绘制黑块轮廓线
            cv::rectangle(gameImage, blockDetector.getBlockRect(), cv::Scalar(0, 0, 255), 2);
            if (isClickDone && (column >= 0))
            {
                // 发送敲击命令
                char col = '0' + column;
                if (column == 0 || column == 1)
                    serial_0.writeData(&col, 1);
                else
                    serial_1.writeData(&col, 1);

                // 在敲击完成前禁止继续敲击
                isClickDone = false;
                printf("发送%d\n", column);
            }

            // 检查敲击是否完成
            updateClickState(serial_0, serial_1, isClickDone);
        }

        // 显示图像
        cv::imshow(CAMERA_WINDOW, cameraImage);
        cv::imshow(GAME_WINDOW, screenImage /* blockDetector.drawBlackBlock() */);
        cv::waitKey(30);
    }

    return 0;
}

void on_playChanged(int isPlaying, void *userData)
{
    std::string info = isPlaying ? "⏸  游戏开始" : "⏺  游戏暂停";
    std::cout << info << std::endl;
}

/** @brief 根据串口消息更新各个电机点击状态
 * @param s0 串口 0
 * @param s1 串口 1
 * @param isClickDones 电机点击状态数组
 */
void updateClickState(Serial &s0, Serial &s1, bool &isClickDone)
{
    std::string msg = s0.readData() + s1.readData();
    if(!msg.empty())
        isClickDone = true;
    /* char stepperNums[5] = "0123";
    for (int i = 0; i < 4; ++i)
        if (msg.find(stepperNums[i]) != msg.npos)
        {
            printf("收到%d\n", i);
            isClickDones[i] = true;
        } */
}