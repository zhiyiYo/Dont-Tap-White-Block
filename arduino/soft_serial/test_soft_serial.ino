#include <SoftwareSerial.h>

SoftwareSerial softSerial(2, 3);

void setup()
{
    softSerial.begin(9600);
}

void loop()
{
    if (softSerial.read() == '1')
    {
        softSerial.print("Soft serial has received data from device:1");
    }
}