#include <Stepper.h>

const int stepsPerRevolution = 200;
int steps = 10;

// initialize the stepper library on pins 8 through 11:
Stepper myStepper(stepsPerRevolution, 8, 9, 10, 11);

void setup()
{
    myStepper.setSpeed(200);
    Serial.begin(9600);
}

void loop()
{
    if (Serial.read() == '1')
    {
        // 正向转动
        myStepper.step(steps);
        delay(50);
        myStepper.step(-steps);
        delay(50);
    }
}