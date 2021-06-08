#include "ScreenDetector.h"
using namespace cv;

int main(int argc, char const *argv[])
{
    ScreenDetector detector;
    VideoCapture camera(1);
    Mat image;

    while (1)
    {
        camera >> image;
        auto screen = detector.getScreen(image, 90, 4000, Size(9, 9));
        imshow("摄像头", detector.drawScreenRect(image));
        imshow("手机屏幕", screen);
        waitKey(30);
    }

    return 0;
}