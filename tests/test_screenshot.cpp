#include "Screenshot.h"
#include <iostream>
using namespace cv;
using namespace std;

int main()
{
    Screenshot screenshot;
    Mat img = screenshot.getScreenshot();
    double t0 = getTickCount();
    Mat img_ = screenshot.getScreenshot(1040, 132, 800, 880);
    cout << "截图耗时：" << ((double)getTickCount() - t0) / getTickFrequency()<< " s" << endl;
    imwrite("screenshot.jpg", img);
    imwrite("screenshot_part.jpg", img_);
    return 0;
}