#include <iostream>
#include "Serial.h"
using namespace std;

int main()
{
    char *a = new char[4];
    Serial mySerial;

    if (!mySerial.initPort(3, 9600, 8, 1, 0))
    {
        cout << "Open COM wrong!" << endl;
    }
    if (!mySerial.openThread())
    {
        cout << "Open thread wrong!";
    }
    a[0] = '0';
    a[1] = 'A';
    a[2] = 'B';
    a[3] = 'C';
    cout << "Send data: " << (mySerial.writeData(a, 4) ? "Successful" : "Fail") << endl;
    system("pause");
    cout << endl;
    return 0;
}