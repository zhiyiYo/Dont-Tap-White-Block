#pragma once
#include <Arduino.h>
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
    void reset();

private:
    State m_state = WAIT_0;
    int m_stepperNum;
    char m_seq_0;
    char m_seq_1;
    Stepper *m_pStepper;
    SoftwareSerial *m_pSerial;
    void stepperClick(int steps, int pressDelay, int releaseDelay);
};
