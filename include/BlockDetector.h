#pragma once

#include <vector>
#include <opencv2/opencv.hpp>
using namespace cv;

class BlockDetector
{
public:
    BlockDetector() {}
    int findBlackBlock(Mat &src);
    int getBlackBlockCounts() const;
    int getPressedColumn() const;
    void drawBlackBlockContours();

private:
    int m_nBlacks = 0;
    int m_pressedColumn = -1;
    int m_pressedContourIndex = -1;
    Mat m_edge, m_grayImage;
    Mat m_kernel = getStructuringElement(MORPH_RECT, Size(3, 3));
    std::vector<std::vector<Point>> m_contours;
};