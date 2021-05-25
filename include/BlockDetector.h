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
    cv::Rect getBlockRect() const;
    void drawBlackBlockContours();

private:
    int m_nBlacks = 0;
    int m_pressedColumn = -1;
    int m_pressedContourIndex = -1;
    cv::Rect m_blockRect;
    cv::Mat m_edge, m_grayImage;
    cv::Mat m_kernel = cv::getStructuringElement(cv::MORPH_RECT, cv::Size(5, 5));
    std::vector<std::vector<cv::Point>> m_contours;
    void reset();
};