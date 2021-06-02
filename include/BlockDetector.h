#pragma once
#include <vector>
#include <opencv2/opencv.hpp>

class BlockDetector
{
public:
    BlockDetector() {}
    int getBlackBlockCounts() const;
    int getPressedColumn() const;
    cv::Rect getBlockRect() const;
    cv::Mat drawBlackBlock();
    int findBlackBlock(const cv::Mat &src,
                       int threshold = 20,
                       int minArea = 9000,
                       cv::Size kernelSize = cv::Size(5, 5));

private:
    int m_nBlacks = 0;
    int m_pressedColumn = -1;
    int m_pressedContourIndex = -1;
    cv::Rect m_blockRect;
    cv::Mat m_binaryImage, m_grayImage,m_edge;
    std::vector<std::vector<cv::Point>> m_contours;
    void reset();
};