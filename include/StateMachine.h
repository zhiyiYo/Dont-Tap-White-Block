#pragma once
#include <iostream>
#include "Timer.h"
#include "Serial.h"

class StateMachine
{
public:
    enum class State
    {
        WAIT_COMMAND_0,
        WAIT_COMMAND_1,
        WAIT_ACK_0,
        WAIT_ACK_1
    };

    enum class MessageType
    {
        COMMAND,
        ACK
    };

    StateMachine(int stepperNum, Serial *pSerial, int timeout = 300);
    bool transition(char msg, MessageType msgType);
    void reset();

private:
    int m_stepperNum;
    char m_seq_0;
    char m_seq_1;
    Serial *m_pSerial;
    int m_timeout;
    Timer m_timer;
    State m_state = State::WAIT_COMMAND_0;
    bool isTimerOverflow(int64_t maxMilliseconds = 300);
};
