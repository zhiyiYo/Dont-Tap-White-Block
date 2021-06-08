#include <Stepper.h>
#include <SoftwareSerial.h>

int stepperNum;
int steps = 5;
int pressDelay = 85;
int releaseDelay = 20;
int stepperSpeed = 100;
SoftwareSerial softSerial(2, 3);
Stepper stepper_0(200, 4, 5, 6, 7);
Stepper stepper_1(200, 8, 9, 10, 11);

void setup()
{
    stepper_0.setSpeed(stepperSpeed);
    stepper_1.setSpeed(stepperSpeed);
    softSerial.begin(9600);
}

void loop()
{
    if (softSerial.available())
    {
        stepperNum = softSerial.read() - '0';
        if (stepperNum == 2)
        {
            stepper_0.step(steps - 1);
            delay(pressDelay);
            stepper_0.step(-steps + 1);
            delay(releaseDelay);
            softSerial.print('2');
        }
        else if (stepperNum == 3)
        {
            stepper_1.step(steps);
            delay(pressDelay);
            stepper_1.step(-steps);
            delay(releaseDelay);
            softSerial.print('3');
        }
    }
}