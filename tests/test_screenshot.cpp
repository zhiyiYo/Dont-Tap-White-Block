#include "Screenshot.h"
#include <iostream>
using namespace cv;
using namespace std;

int main()
{
    Screenshot screenshot(1920, 1080);
    Mat img = screenshot.getScreenshot();
    double t0 = getTickCount();
    Mat img_ = screenshot.getScreenshot(500, 40, 600, 600);
    cout << "截图耗时：" << ((double)getTickCount() - t0) / getTickFrequency()<< " s" << endl;
    imwrite("screenshot.jpg", img);
    imwrite("screenshot_part.jpg", img_);
    return 0;
}