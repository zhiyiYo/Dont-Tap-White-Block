#include <Stepper.h>
#include <SoftwareSerial.h>

int stepperNum;
int steps = 20;
SoftwareSerial softSerial(2, 3);
Stepper stepper_0(200, 8, 9, 10, 11);
Stepper stepper_1(200, 4, 5, 6, 7);

void setup()
{
    stepper_0.setSpeed(200);
    stepper_1.setSpeed(200);
    softSerial.begin(9600);
}

void loop()
{
    if (softSerial.available())
    {
        stepperNum = (softSerial.read() - '0') % 2;
        if (stepperNum == 0)
        {
            stepper_0.step(steps);
            delay(100);
            stepper_0.step(-steps);
            delay(100);
        }
        else
        {
            stepper_1.step(steps);
            delay(100);
            stepper_1.step(-steps);
            delay(100);
        }
        softSerial.print('d');
    }
}