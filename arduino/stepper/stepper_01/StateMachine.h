#pragma once
#include <Stepper.h>
#include <SoftwareSerial.h>

class StateMachine
{
public:
    enum State
    {
        WAIT_0 = 0,
        WAIT_1 = 1
    };
    StateMachine(int stepperNum, Stepper *pStepper, SoftwareSerial *pSerial);
    void transition(char msg, int steps = 5, int pressDelay = 90, int releaseDelay = 20);

private:
    State m_state;
    int m_stepperNum;
    Stepper *m_pStepper;
    SoftwareSerial *m_pSerial;
    void stepperClick(int steps, int pressDelay, int releaseDelay);
};
