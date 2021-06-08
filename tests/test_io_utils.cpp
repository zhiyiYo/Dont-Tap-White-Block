#include <iostream>
#include "IOUtils.h"
using namespace std;

int main(int argc, char const *argv[])
{
    cout << "按 `S` 鼠标左键点击\n按 `Q` 退出\n"
         << endl;

    while (1)
    {
        char key = getch();
        if (key)
        {
            cout << "按下的键为: " << key << endl;
            if (key == 's')
                mouseClick(0, 30);
            else if (key == 'q')
                break;
        }

        Sleep(200);
    }

    return 0;
}