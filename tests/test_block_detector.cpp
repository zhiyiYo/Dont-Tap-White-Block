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

    cv::Mat image;
    BlockDetector detector;

    for (int i = 0; i < imPaths.size(); ++i)
    {
        image = cv::imread(imPaths[i]);
        assertEqual<int>(i-1, detector.findBlackBlock(image));
    }

    // 绘制轮廓线
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