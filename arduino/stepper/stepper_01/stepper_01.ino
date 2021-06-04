#include <Stepper.h>
#include <SoftwareSerial.h>

int stepperNum;
int steps = 5;
int stepDelay = 100;
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
        if (stepperNum == 0)
        {
            stepper_0.step(-steps);
            delay(stepDelay);
            stepper_0.step(steps);
            delay(stepDelay);
            softSerial.print('0');
        }
        else if (stepperNum == 1)
        {
            stepper_1.step(-steps + 1);
            delay(stepDelay);
            stepper_1.step(steps - 1);
            delay(stepDelay);
            softSerial.print('1');
        }
    }
}