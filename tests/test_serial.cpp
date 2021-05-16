#include <iostream>
#include "Serial.h"
using namespace std;

int main()
{
    char a[] = "0ABC";
    Serial mySerial;

    if (!mySerial.initPort(5, 9600, 8, 1, 0))
    {
        cout << "Open COM wrong!" << endl;
    }
    if (!mySerial.openThread())
    {
        cout << "Open thread wrong!";
    }
    cout << "Send data: " << (mySerial.writeData(a, 4) ? "Successful" : "Fail") << endl;
    system("pause");
    cout << endl;
    return 0;
}