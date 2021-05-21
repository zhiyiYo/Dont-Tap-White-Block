#include"IOUtils.h"

/* 获取按下的键，没有按下的键则返回空字符 */
char getPressedKey()
{
    return kbhit() ? getch() : '\0';
}

/** @brief 将鼠标移动到指定位置并按下鼠标
 * @param x 坐标 x
 * @param y 坐标 y
 */
void mouseEvent(int x, int y)
{
    SetCursorPos(x, y);
    mouse_event(MOUSEEVENTF_LEFTDOWN, 0, 0, 0, 0);
    mouse_event(MOUSEEVENTF_LEFTUP, 0, 0, 0, 0);
}