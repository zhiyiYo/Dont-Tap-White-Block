#include "BlockDetector.h"

/** @brief 在输入图像中寻找应该被点击的黑块所在的列
 * @param img 输入图像
 * @return 应该被点击的列，取值 0 ~ 3，如果没有找到返回 -1
 */
int BlockDetector::findBlackBlock(Mat &img)
{
    cvtColor(img, m_grayImage, COLOR_BGR2GRAY);
    //auto t0 = getTickCount();

    // 提取轮廓线
    dilate(m_grayImage, m_grayImage, m_kernel, Point(-1, -1), 2); // 图像膨胀消去网格线并分离分块
    m_grayImage = m_grayImage > 80;                               // 图像阈值化消去已点击过的灰色方块
    Canny(m_grayImage, m_edge, 10, 30);                           // Canny 算法进行边缘提取
    findContours(m_edge, m_contours, RETR_EXTERNAL, CHAIN_APPROX_SIMPLE);

    // 获取图像中的黑块个数
    m_nBlacks = m_contours.size();

    // 根据轮廓线的外接斜矩形的中心 y 判断点击位置
    if (m_nBlacks)
    {
        int yMax = 0, x = 0;
        for (int i = 0; i < m_nBlacks; ++i)
        {
            // 创建最小斜矩形
            RotatedRect rect = minAreaRect(m_contours[i]);
            if (rect.center.y >= yMax)
            {
                yMax = rect.center.y;
                x = rect.center.x;
                m_pressedContourIndex = i;
            }
        }

        m_pressedColumn = x / (img.cols / 4);
    }
    else
    {
        m_pressedColumn = -1;
    }

    // std::cout << "运行时间为：" << (getTickCount() - t0) / getTickFrequency() << std::endl;
    return m_pressedColumn;
}

/* 绘制黑块轮廓线，红色边框包围的是应该被点击的黑块 */
void BlockDetector::drawBlackBlockContours()
{
    Mat dst = m_grayImage.clone();
    cvtColor(dst, dst, COLOR_GRAY2BGR);

    // 绘制轮廓线
    for (int i = 0; i < m_nBlacks; ++i)
    {
        Scalar color(0, i == m_pressedContourIndex ? 0 : 255, 255);
        drawContours(dst, m_contours, i, color, 2, LINE_AA);
    }

    // 显示图像
    imshow("Detect result", dst);
    waitKey(0);
}

/* 返回黑块个数 */
int BlockDetector::getBlackBlockCounts() const
{
    return m_nBlacks;
}

/* 返回被点击的列 */
int BlockDetector::getPressedColumn() const
{
    return m_pressedColumn;
}