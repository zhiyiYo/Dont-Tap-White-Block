#include "BlockDetector.h"
using namespace std;

template <typename T>
void assertEqual(T expect, T actual);

int main(int argc, char const *argv[])
{
    vector<string> imPaths{
        "resource/images/-1.png",
        "resource/images/0.png",
        "resource/images/1.png",
        "resource/images/2.png",
        "resource/images/3.png"};

    vector<Mat> images(imPaths.size());
    BlockDetector detector;

    for (int i = 0; i < images.size(); ++i)
    {
        images[i] = imread(imPaths[i]);
    }

    assertEqual<int>(-1, detector.findBlackBlock(images[0]));
    assertEqual<int>(0, detector.findBlackBlock(images[1]));
    assertEqual<int>(1, detector.findBlackBlock(images[2]));
    assertEqual<int>(2, detector.findBlackBlock(images[3]));
    assertEqual<int>(3, detector.findBlackBlock(images[4]));

    detector.drawBlackBlockContours();

    return 0;
}

template <typename T>
void assertEqual(T expect, T actual)
{
    string result = expect == actual ? " ✅" : " ❌";
    cout << result
         << " Expect: " << setw(2) << expect
         << ", Get: " << setw(2) << actual << endl;
}