#include "StateMachine.h"

int steps = 4;
int pressDelay = 50;
int releaseDelay = 50;
int stepperSpeed = 100;
SoftwareSerial softSerial(2, 3);
Stepper stepper_2(200, 4, 5, 6, 7);
Stepper stepper_3(200, 8, 9, 10, 11);
StateMachine stateMachine_2(2, &stepper_2, &softSerial);
StateMachine stateMachine_3(3, &stepper_3, &softSerial);

void setup()
{
    stepper_2.setSpeed(stepperSpeed);
    stepper_3.setSpeed(stepperSpeed);
    softSerial.begin(9600);
}

void loop()
{
    if (softSerial.available())
    {
        char msg = softSerial.read();
        if (msg == '2' || msg == '6')
            stateMachine_2.transition(msg, -steps + 2, pressDelay, releaseDelay);
        else if (msg == '3' || msg == '7')
            stateMachine_3.transition(msg, -steps, pressDelay, releaseDelay);
        else if (msg == 'r')
        {
            stateMachine_2.reset();
            stateMachine_3.reset();
        }
    }
}