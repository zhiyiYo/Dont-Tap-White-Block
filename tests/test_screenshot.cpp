#include "Screenshot.h"
#include <iostream>
using namespace cv;
using namespace std;

int main()
{
    Screenshot screenshot;
    Mat img = screenshot.getScreenshot();
    Mat img_ = screenshot.getScreenshot(1040, 132, 800, 880);
    imwrite("screenshot.jpg", img);
    imwrite("screenshot_part.jpg", img_);
    return 0;
}