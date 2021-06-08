#include <opencv2/opencv.hpp>
using namespace cv;

int main(int argc, char const *argv[])
{
    VideoCapture camera(1);
    Mat image;
    while (1)
    {
        camera >> image;
        imshow("camera", image);
        waitKey(30);
    }
    return 0;
}