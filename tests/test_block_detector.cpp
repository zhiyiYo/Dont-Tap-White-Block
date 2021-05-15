#include "BlockDetector.h"
using namespace std;

int main(int argc, char const *argv[])
{
    Mat src = imread("resource/images/1.png");
    BlockDetector detector;

    int column = detector.findBlackBlock(src);
    detector.drawBlackBlockContours();
    cout << "应该点击的列：" << column << endl;

    return 0;
}
