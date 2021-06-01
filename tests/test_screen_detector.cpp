#include "ScreenDetector.h"
using cv::Mat;

int main(int argc, char const *argv[])
{
    ScreenDetector detector;
    cv::VideoCapture camera(1);
    Mat image;

    while (1)
    {
        camera >> image;
        cv::Rect rect = detector.findScreenRect(image, 90, 5000, cv::Size(2, 2));
        cv::imshow("camera", detector.drawScreenRect(image));
        //std::cout << detector.getRotateScreenRect().angle << ", ";
        cv::waitKey(40);
    }

    return 0;
}