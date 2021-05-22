#include "BlockDetector.h"
using namespace std;
using namespace cv;

template <typename T>
void assertEqual(T expect, T actual);

int main(int argc, char const *argv[])
{
    vector<string> imPaths{
        "resource/images/开始.png",
        "resource/images/-1.png",
        "resource/images/0.png",
        "resource/images/1.png",
        "resource/images/2.png",
        "resource/images/3.png",
        "resource/images/结束.png",
        "resource/images/结束.jpg"
        };

    vector<int> answers{1, -1, 0, 1, 2, 3, -1, -1};
    Mat image;
    BlockDetector detector;

    for (size_t i = 0; i < imPaths.size(); ++i)
    {
        image = imread(imPaths[i]);
        assertEqual<int>(answers[i], detector.findBlackBlock(image));
    }

    // 绘制轮廓线
    detector.drawBlackBlockContours();

    return 0;
}

template <typename T>
void assertEqual(T expect, T actual)
{
    string result = expect == actual ? "✅ " : "❌ ";
    cout << result
         << "Expect: " << setw(2) << expect
         << ", Get: " << setw(2) << actual << endl;
}