#include "StateMachine.h"

int steps = 4;
int pressDelay = 50;
int releaseDelay = 50;
int stepperSpeed = 100;
SoftwareSerial softSerial(2, 3);
Stepper stepper_0(200, 4, 5, 6, 7);
Stepper stepper_1(200, 8, 9, 10, 11);
StateMachine stateMachine_0(0, &stepper_0, &softSerial);
StateMachine stateMachine_1(1, &stepper_1, &softSerial);

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
        char msg = softSerial.read();
        if (msg == '0' || msg == '4')
            stateMachine_0.transition(msg, steps, pressDelay, releaseDelay);
        else if (msg == '1' || msg == '5')
            stateMachine_1.transition(msg, steps - 2, pressDelay, releaseDelay);
        else if (msg == 'r')
        {
            stateMachine_0.reset();
            stateMachine_1.reset();
        }
    }
}