#include <Stepper.h>

const int stepsPerRevolution = 200;
int steps = 200;

// initialize the stepper library on pins 8 through 11:
Stepper myStepper(stepsPerRevolution, 8, 9, 10, 11);

void setup()
{
    myStepper.setSpeed(100);
    Serial.begin(9600);
}

void loop()
{
    if (Serial.read() == '1')
    {
        // 正向转动
        myStepper.step(steps);
    }
}