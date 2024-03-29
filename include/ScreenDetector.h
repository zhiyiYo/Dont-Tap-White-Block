#pragma once
#include <vector>
#include <opencv2/opencv.hpp>

class ScreenDetector
{
public:
    ScreenDetector() {}
    cv::Mat getScreen(const cv::Mat &image,
                      int threshold = 125,
                      int minArea = 10000,
                      cv::Size kernelSize = cv::Size(2, 2));
    cv::Rect getScreenRect() const;
    cv::RotatedRect getRotateScreenRect() const;
    cv::Mat drawScreenRect(const cv::Mat &image);

private:
    cv::Rect m_screenRect;
    cv::RotatedRect m_rotateScreenRect;
    cv::Mat m_binaryImage, m_grayImage;
    std::vector<std::vector<cv::Point>> m_contours;
    cv::Rect findScreenRect(const cv::Mat &image,
                            int threshold = 125,
                            int minArea = 10000,
                            cv::Size kernelSize = cv::Size(2, 2));
};