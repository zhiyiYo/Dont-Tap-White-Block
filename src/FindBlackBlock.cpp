#include <vector>
#include <opencv2/opencv.hpp>
using namespace cv;
using namespace std;

int main(int argc, char const *argv[])
{
    string imagePath = "resource/images/1.png";
    Mat src = imread(imagePath);
    Mat graySrc = imread(imagePath, IMREAD_GRAYSCALE);
    Mat kernel = getStructuringElement(MORPH_RECT, Size(3, 3));
    Mat edge;

    // 轮廓线
    vector<vector<Point>> contours;

    // 提取轮廓线
    auto t0 = getTickCount();
    dilate(graySrc, graySrc, kernel, Point(-1, -1), 2); // 图像膨胀消去网格线并分离分块
    graySrc = graySrc > 80;                             // 图像阈值化消去已点击过的灰色方块
    Canny(graySrc, edge, 10, 30);
    findContours(edge, contours, RETR_EXTERNAL, CHAIN_APPROX_SIMPLE);

    int nBlacks = contours.size();
    cout << "黑块个数：" << nBlacks << endl;

    // 根据轮廓线的外接斜矩形的中心 y 判断点击位置
    if (nBlacks)
    {
        int yMax = 0, x = 0;
        for (int i = 0; i < nBlacks; ++i)
        {
            // 创建最小斜矩形
            RotatedRect rect = minAreaRect(contours[i]);
            if (rect.center.y >= yMax)
            {
                yMax = rect.center.y;
                x = rect.center.x;
            }
        }
        cout << "应该点击的列：" << x / (src.cols / 4) << endl;
        // 绘制应该点击的方块的中心点
        circle(src, Point(x, yMax), 5, Scalar(0, 0, 255), FILLED);
    }
    cout << "运行时间为：" << (getTickCount() - t0) / getTickFrequency() << endl;

    // 绘制轮廓线
    for (int i = 0; i < nBlacks; ++i)
        drawContours(src, contours, i, Scalar(0, 0, 255), 2, LINE_AA);

    imshow("Detect result", src);
    imshow("Gray image", graySrc);
    imshow("Edges", edge);
    waitKey(0);

    return 0;
}