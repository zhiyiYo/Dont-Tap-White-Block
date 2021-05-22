#include <iostream>
#include "IOUtils.h"
using namespace std;

int main(int argc, char const *argv[])
{
    cout << "按 `Q` 鼠标左键点击\n"
         << endl;

    while (1)
    {
        char key = getch();
        if (key)
        {
            cout << "按下的键为: " << key << endl;
            if (key == 'q')
                mouseClick(0, 30);
        }

        Sleep(200);
    }

    return 0;
}