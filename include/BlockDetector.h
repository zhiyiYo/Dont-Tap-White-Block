#pragma once

#include <vector>
#include <opencv2/opencv.hpp>

class BlockDetector
{
public:
    BlockDetector() {}
    int findBlackBlock(cv::Mat &src);
    int getBlackBlockCounts() const;
    int getPressedColumn() const;
    void drawBlackBlockContours();

private:
    int m_nBlacks = 0;
    int m_pressedColumn = -1;
    int m_pressedContourIndex = -1;
    cv::Mat m_edge, m_grayImage;
    cv::Mat m_kernel = cv::getStructuringElement(cv::MORPH_RECT, cv::Size(3, 3));
    std::vector<std::vector<cv::Point>> m_contours;
};