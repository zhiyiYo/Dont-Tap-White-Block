#include "ScreenDetector.h"
using cv::Mat;
using cv::Point2f;

int main(int argc, char const *argv[])
{
    ScreenDetector detector;
    cv::VideoCapture camera(1);
    Mat image;

    while (1)
    {
        camera >> image;
        auto screen = detector.getScreen(image, 90, 4000, cv::Size(9, 9));
        cv::imshow("摄像头", detector.drawScreenRect(image));
        cv::imshow("手机屏幕", screen);
        cv::waitKey(30);
    }

    return 0;
}