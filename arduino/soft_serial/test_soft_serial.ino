#include <SoftwareSerial.h>

char data;
SoftwareSerial softSerial(2, 3);

void setup()
{
    softSerial.begin(9600);
}

void loop()
{
    if (softSerial.available())
    {
        data = softSerial.read();
        if (data == '1')
        {
            softSerial.print("Soft serial has received data from device:2");
        }
    }
}