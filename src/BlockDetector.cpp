#include "BlockDetector.h"
using cv::Mat;

/** @brief 在输入图像中寻找应该被点击的黑块所在的列
 * @param img 输入图像
 * @return 应该被点击的列，取值 0 ~ 3，如果没有找到返回 -1
 */
int BlockDetector::findBlackBlock(Mat &img)
{
    cv::cvtColor(img, m_grayImage, cv::COLOR_BGR2GRAY);
    cv::copyMakeBorder(m_grayImage, m_grayImage, 1, 1, 1, 1, cv::BORDER_CONSTANT, cv::Scalar(255));

    // auto t0 = cv::getTickCount();
    // 提取轮廓线
    cv::dilate(m_grayImage, m_grayImage, m_kernel, cv::Point(-1, -1), 2); // 图像膨胀消去网格线并分离分块
    m_grayImage = m_grayImage > 60;                                       // 图像阈值化消去已点击过的灰色方块
    cv::Canny(m_grayImage, m_edge, 10, 30);                               // Canny 算法进行边缘提取
    cv::findContours(m_edge, m_contours, cv::RETR_EXTERNAL, cv::CHAIN_APPROX_SIMPLE);

    // 只保留面积小于阈值的轮廓
    decltype(m_contours) contours;
    for (auto &i : m_contours)
    {
        if (cv::contourArea(i) >= 10000)
            contours.push_back(i);
    }

    // 获取图像中的黑块个数并初始化点击的列
    m_nBlacks = contours.size();

    // 根据轮廓线的外接斜矩形的中心 y 判断点击位置
    if (m_nBlacks > 0)
    {
        int yMax = 0, x = 0;
        for (int i = 0; i < m_nBlacks; ++i)
        {
            // 创建外接矩形
            cv::Rect rect = cv::boundingRect(contours[i]);
            if (rect.y >= yMax)
            {
                x = rect.x;
                yMax = rect.y;
                m_pressedContourIndex = i;
                m_blockRect = rect;
            }
        }

        m_pressedColumn = x / (img.cols / 4);
    }
    else
    {
        reset();
    }

    // std::cout << "运行时间为：" << (cv::getTickCount() - t0) / cv::getTickFrequency() << std::endl;
    return m_pressedColumn;
}

/* 绘制黑块轮廓线，红色边框包围的是应该被点击的黑块 */
void BlockDetector::drawBlackBlockContours()
{
    Mat dst = m_grayImage.clone();
    cv::cvtColor(dst, dst, cv::COLOR_GRAY2BGR);

    // 绘制轮廓线
    for (int i = 0; i < m_nBlacks; ++i)
    {
        cv::Scalar color(0, i == m_pressedContourIndex ? 0 : 255, 255);
        cv::drawContours(dst, m_contours, i, color, 2, cv::LINE_AA);
    }

    // 显示图像
    cv::imshow("Detect result", dst);
    cv::waitKey(0);
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

/* 返回黑块的外接矩形 */
cv::Rect BlockDetector::getBlockRect() const
{
    return m_blockRect;
}

/* 重置成员 */
void BlockDetector::reset()
{
    m_pressedColumn = -1;
    m_pressedContourIndex = -1;
    m_blockRect = cv::Rect();
}