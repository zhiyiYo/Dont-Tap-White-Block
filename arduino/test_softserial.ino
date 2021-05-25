#include <SoftwareSerial.h>

SoftwareSerial myserial(2, 3);

void setup()
{
    myserial.begin(9600);
}

void loop()
{
    myserial.print("ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789~!@#$%^&*()-");
    delay(1000);
    //Serial.println("A");
    delay(1000);
}