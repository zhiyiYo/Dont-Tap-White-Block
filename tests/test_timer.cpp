#include <iostream>
#include <windows.h>
#include "Timer.h"

int main(int argc, char const *argv[])
{
    Timer timer;
    timer.start();
    Sleep(102);
    auto duration = timer.getDuration();
    std::cout << "耗时：" << duration << "毫秒" << std::endl;
    return 0;
}