#include "Screenshot.h"
using cv::Mat;

/** @brief 创建截屏对象
 * @param width 显示屏分辨率宽度
 * @param height 显示屏分辨率高度
 */
Screenshot::Screenshot(int width, int height)
{
    m_width = width;
    m_height = height;
    m_screenshotData = new char[width * height * 4];
    memset(m_screenshotData, 0, width);

    // 获取屏幕 DC
    m_screenDC = GetDC(NULL);
    m_compatibleDC = CreateCompatibleDC(m_screenDC);

    // 创建位图
    m_hBitmap = CreateCompatibleBitmap(m_screenDC, width, height);
    SelectObject(m_compatibleDC, m_hBitmap);
}

/* 获取整个屏幕的截图 */
Mat Screenshot::getScreenshot()
{
    // 得到位图的数据
    BitBlt(m_compatibleDC, 0, 0, m_width, m_height, m_screenDC, 0, 0, SRCCOPY);
    GetBitmapBits(m_hBitmap, m_width * m_height * 4, m_screenshotData);

    // 创建图像
    Mat screenshot(m_height, m_width, CV_8UC4, m_screenshotData);

    return screenshot;
}

/** @brief 获取指定范围的屏幕截图
 * @param x 图像左上角的 X 坐标
 * @param y 图像左上角的 Y 坐标
 * @param width 图像宽度
 * @param height 图像高度
 */
Mat Screenshot::getScreenshot(int x, int y, int width, int height)
{
    Mat screenshot = getScreenshot();
    return screenshot(cv::Rect(x, y, width, height));
}
