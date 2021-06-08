#include "Serial.h"
#include "BlockDetector.h"
#include "ScreenDetector.h"
#define CAMERA_WINDOW "camera"
#define GAME_WINDOW "game"

void on_playChanged(int isPlaying, void *userData);
void updateClickState(Serial &serial_0, Serial &serial_1, bool &isClickDone);

int main(int argc, char const *argv[])
{
    // 开始游戏标志位
    int isPlaying = 0;

    // 打开 USB 摄像头
    cv::VideoCapture camera(1);
    cv::Mat cameraImage, gameImage, screenImage;

    // 创建黑块探测器和手机屏幕探测器
    BlockDetector blockDetector;
    ScreenDetector screenDetector;

    // 创建串口实例
    Serial serial_0, serial_1;
    int port_0 = argc == 1 ? 8 : atoi(argv[1]);
    int port_1 = argc <= 2 ? 9 : atoi(argv[2]);
    if (!serial_0.initPort(port_0, 9600, 8, 1, 0))
    {
        std::cerr << "打开串口 COM" << port_0 << " 失败！" << std::endl;
        return -1;
    };
    if (!serial_1.initPort(port_1, 9600, 8, 1, 0))
    {
        std::cerr << "打开串口 COM" << port_1 << " 失败！" << std::endl;
        return -1;
    };

    // 创建图窗
    cv::namedWindow(CAMERA_WINDOW);
    cv::namedWindow(GAME_WINDOW);
    cv::createTrackbar("Play game", CAMERA_WINDOW, &isPlaying, 1, on_playChanged, NULL);

    // 敲击是否结束标志位
    bool isClickDone = true;

    // 读入摄像头的拍摄内容
    while (1)
    {
        camera >> cameraImage;

        // 检测手机屏幕
        cv::Size kernelSize(11, 11);
        screenImage = screenDetector.getScreen(cameraImage, 125, 5000, kernelSize);
        cameraImage = screenDetector.drawScreenRect(cameraImage); // 绘制手机轮廓

        // 截取游戏区域
        cv::Rect gameRegion(kernelSize.width + 1,
                            screenImage.rows / 2,
                            screenImage.cols - 2 * kernelSize.width,
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
            if (column >= 0 && isClickDone)
            {
                // 发送敲击命令
                char col = '0' + column;
                if (col == '0' || col == '1')
                    serial_0.writeData(&col, 1);
                else
                    serial_1.writeData(&col, 1);

                // 在敲击完成前禁止继续敲击
                isClickDone = false;
            }

            // 检查敲击是否完成
            updateClickState(serial_0, serial_1, isClickDone);
        }

        // 显示图像
        cv::imshow(CAMERA_WINDOW, cameraImage);
        cv::imshow(GAME_WINDOW, screenImage);
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
 * @param isClickDone 电机点击状态
 */
void updateClickState(Serial &s0, Serial &s1, bool &isClickDone)
{
    std::string col = s0.readData() + s1.readData();
    if (!col.empty())
        isClickDone = true;
}