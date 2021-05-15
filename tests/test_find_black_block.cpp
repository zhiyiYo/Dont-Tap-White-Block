#include "BlockDetector.h"
using namespace std;

int main(int argc, char const *argv[])
{
    string imagePath = "resource/images/1.png";
    Mat src = imread(imagePath);
    BlockDetector detector;
    int column = detector.findBlackBlock(src);
    detector.drawBlackBlockContours();
    return 0;
}
