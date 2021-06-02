#include "BlockDetector.h"
using cv::Mat;

/** @brief 在输入图像中寻找应该被点击的黑块所在的列
 * @param img 输入图像
 * @param threshold 黑块的最大灰度值
 * @param minArea 黑块的最小面积
 * @param kernelSize 图像膨胀的卷积核大小
 * @return 应该被点击的列，取值 0 ~ 3，如果没有找到返回 -1
 */
int BlockDetector::findBlackBlock(const Mat &img, int threshold, int minArea, cv::Size kernelSize)
{
    // auto t0 = cv::getTickCount();

    cv::cvtColor(img, m_grayImage, cv::COLOR_BGR2GRAY);
    cv::copyMakeBorder(m_grayImage, m_grayImage, 1, 1, 1, 1, cv::BORDER_CONSTANT, cv::Scalar(255));

    Mat kernel = cv::getStructuringElement(cv::MORPH_RECT, kernelSize);

    // 提取轮廓线
    m_binaryImage = m_grayImage > threshold;                                // 图像阈值化消去已点击过的灰色方块
    cv::dilate(m_binaryImage, m_binaryImage, kernel, cv::Point(-1, -1), 2); // 图像膨胀消去网格线并分离分块
    cv::Canny(m_binaryImage, m_edge, 10, 30);                               // Canny 算法进行边缘提取
    cv::findContours(m_edge, m_contours, cv::RETR_TREE, cv::CHAIN_APPROX_SIMPLE);

    // 只保留面积大于阈值的轮廓
    decltype(m_contours) contours;
    for (auto &i : m_contours)
    {
        if (cv::contourArea(i) >= minArea)
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

        m_pressedColumn = (x + m_blockRect.width / 2) / (img.cols / 4);
    }
    else
    {
        reset();
    }

    // std::cout << "运行时间为：" << (cv::getTickCount() - t0) / cv::getTickFrequency() << std::endl;
    return m_pressedColumn;
}

/* 绘制应该被点击的黑块的外接矩形 */
Mat BlockDetector::drawBlackBlock()
{
    Mat dst = m_binaryImage.clone();
    cv::cvtColor(dst, dst, cv::COLOR_GRAY2BGR);

    // 绘制外接矩形
    if (m_nBlacks > 0)
    {
        cv::Scalar color(0, 0, 255);
        cv::rectangle(dst, m_blockRect, color, 2, cv::LINE_AA);
    }
    return dst;
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