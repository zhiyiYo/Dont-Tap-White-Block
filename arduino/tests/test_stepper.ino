#include <Stepper.h>

int stepperNum;
int steps = 200;
Stepper stepper_0(200, 8, 9, 10, 11);

void setup()
{
    stepper_0.setSpeed(100);
}

void loop()
{
    stepper_0.step(steps);
    delay(2000);
    stepper_0.step(-steps);
    delay(2000);
}