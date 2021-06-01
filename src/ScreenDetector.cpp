#include "ScreenDetector.h"
using cv::Mat;
using cv::Rect;

/** @brief 寻找手机屏幕的外接矩形
 * @param image 输入图像，类型为 BGR
 * @param threshold 图像二值化阈值
 * @param minArea 黑块的最小面积
 * @param kernelSize 图像膨胀卷积核大小
 */
Rect ScreenDetector::findScreenRect(const Mat &image, int threshold, int minArea, cv::Size kernelSize)
{
    Mat kernel = cv::getStructuringElement(cv::MORPH_RECT, kernelSize);

    // 图像二值化
    cv::cvtColor(image, m_grayImage, cv::COLOR_BGR2GRAY);
    m_binaryImage = m_grayImage > threshold;

    // 提取轮廓线
    cv::dilate(m_binaryImage, m_binaryImage, kernel, cv::Point(-1, -1), 2);
    cv::findContours(m_binaryImage, m_contours, cv::RETR_EXTERNAL, cv::CHAIN_APPROX_SIMPLE);

    // 剔除面积较小的区域
    decltype(m_contours) contours;
    for (auto &i : m_contours)
    {
        if (cv::contourArea(i) >= minArea)
            contours.push_back(i);
    }

    // 找出面积最大的那个矩形
    size_t num = contours.size();
    if (num > 0)
    {
        m_rotateScreenRect = cv::minAreaRect(contours[0]);
        //m_screenRect = cv::boundingRect(contours[0]);
        for (size_t i = 0; i < num; ++i)
        {
            // auto rect = cv::boundingRect(contours[i]);
            auto rect = cv::minAreaRect(contours[i]);
            if (rect.boundingRect().area() > m_rotateScreenRect.boundingRect().area())
                m_rotateScreenRect = rect;
        }
    }
    else
    {
        m_rotateScreenRect = cv::RotatedRect();
    }
    m_screenRect = m_rotateScreenRect.boundingRect();
    return m_screenRect;
}

/* 绘制手机屏幕的边框 */
Mat ScreenDetector::drawScreenRect(const Mat &image)
{
    Mat dst = image.clone();
    /* Mat dst = m_binaryImage.clone();
    cv::cvtColor(dst, dst, cv::COLOR_GRAY2BGR); */

    cv::Point2f points[4];
    m_rotateScreenRect.points(points);
    for (int i = 0; i < 4; ++i)
    {
        cv::Point2f p1 = points[i];
        cv::Point2f p2 = points[(i + 1) % 4];
        cv::line(dst, p1, p2, cv::Scalar(0, 255, 0), 2, cv::LINE_AA);
    }
    return dst;
}

cv::RotatedRect ScreenDetector::getRotateScreenRect() const
{
    return m_rotateScreenRect;
}