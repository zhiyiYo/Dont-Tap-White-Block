#include "Serial.h"
#include "StateMachine.h"
#include "BlockDetector.h"
#include "ScreenDetector.h"
#define CAMERA_WINDOW "camera"
#define GAME_WINDOW "game"

int seqNumbers[4] = {0, 0, 0, 0};

void on_playChanged(int isPlaying, void *userData);
void resetStateMachines(StateMachine *stateMachines, int *seqNumbers);
void updateClickState(Serial &s0, Serial &s1, StateMachine *stateMachines, int *seqNumbers);

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
    int port_0 = argc == 1 ? 7 : atoi(argv[1]);
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

    // 创建有限状态机和确认号
    StateMachine stateMachines[4] = {
        StateMachine(0, &serial_0, 400),
        StateMachine(1, &serial_0, 400),
        StateMachine(2, &serial_1, 400),
        StateMachine(3, &serial_1, 400),
    };

    // 初始化状态机
    resetStateMachines(stateMachines, seqNumbers);

    // 创建图窗
    cv::namedWindow(CAMERA_WINDOW);
    cv::namedWindow(GAME_WINDOW);
    cv::createTrackbar("Play game", CAMERA_WINDOW, &isPlaying, 1, on_playChanged, static_cast<void *>(stateMachines));

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
            // 寻找黑块并绘制轮廓线
            int column = blockDetector.findBlackBlock(gameImage, 180, gameRegion.area() / 8, cv::Size(2, 2));
            cv::rectangle(gameImage, blockDetector.getBlockRect(), cv::Scalar(0, 0, 255), 2);
            // 没有找到黑块时发送无效指令
            char msg = column >= 0 ? column + seqNumbers[column] * 4 + '0' : '\0';

            // 状态转移
            for (int i = 0; i < 4; ++i)
                stateMachines[i].transition(msg, StateMachine::MessageType::COMMAND);

            // 检查是否收到回复消息
            updateClickState(serial_0, serial_1, stateMachines, seqNumbers);
        }

        // 显示图像
        cv::imshow(CAMERA_WINDOW, cameraImage);
        cv::imshow(GAME_WINDOW, screenImage);
        cv::waitKey(15);
    }

    return 0;
}

void on_playChanged(int isPlaying, void *userData)
{
    if (isPlaying)
    {
        std::cout << "⏸  游戏开始" << std::endl;
    }
    else
    {
        std::cout << "⏺  游戏暂停" << std::endl;
        // 重置状态机
        resetStateMachines(static_cast<StateMachine *>(userData), seqNumbers);
    }
}

/** @brief 根据串口消息更新各个状态机的状态
 * @param s0 串口 0
 * @param s1 串口 1
 * @param stateMachines 有限状态机数组
 * @param seqNumbers 确认号数组
 */
void updateClickState(Serial &s0, Serial &s1, StateMachine *stateMachines, int *seqNumbers)
{
    std::string col = s0.readData() + s1.readData();
    char msg[] = "01234567";
    for (int i = 0; i < 8; ++i)
        if (col.find(msg[i]) != col.npos)
        {
            // 更新状态机，如果成功发生状态转移（有时会收到延迟到来的重复的确认包），就前滚确认号
            int j = i < 4 ? i : i - 4;
            if (stateMachines[j].transition(msg[i], StateMachine::MessageType::ACK))
                seqNumbers[j] = (seqNumbers[j] + 1) % 2;
        }
}

/* 重置所有状态机 */
void resetStateMachines(StateMachine *stateMachines, int *seqNumbers)
{
    for (int i = 0; i < 4; ++i)
    {
        stateMachines[i].reset();
        seqNumbers[i] = 0;
    }
}