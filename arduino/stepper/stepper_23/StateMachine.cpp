#include "StateMachine.h"

/** @brief 创建有限状态机
 * @param stepperNum 电机编号，范围为 0 ~ 3
 * @param pStepper 步进电机指针
 * @param pSerial 蓝牙串口指针
 */
StateMachine::StateMachine(int stepperNum, Stepper *pStepper, SoftwareSerial *pSerial)
{
    m_stepperNum = stepperNum;
    m_seq_0 = '0' + stepperNum;
    m_seq_1 = '4' + stepperNum;
    m_pStepper = pStepper;
    m_pSerial = pSerial;
}

/** @brief 电机进行状态转移
 * @param msg 串口读出的控制信号，0 ~ 3 代表 seq0，4 ~ 7 代表 seq1
 * @param steps 转动步数
 * @param pressDelay 按下时长
 * @param releaseDeleay 抬起时长
 */
void StateMachine::transition(char msg, int steps, int pressDelay, int releaseDelay)
{
    if (msg != m_seq_0 && msg != m_seq_1)
        return;

    bool hasSeq_0 = msg < '4';
    bool hasSeq_1 = !hasSeq_0;
    if (m_state == WAIT_0)
    {
        if (hasSeq_0)
        {
            // 收到正确确认号的点击指令，发生点击并回复点击完成
            stepperClick(steps, pressDelay, releaseDelay);
            m_pSerial->print(m_seq_0);
            m_state = WAIT_1;
        }
        else
        {
            // 收到冗余指令，直接回复点击完成
            m_pSerial->print(m_seq_1);
        }
    }
    else
    {
        if (hasSeq_1)
        {
            stepperClick(steps, pressDelay, releaseDelay);
            m_pSerial->print(m_seq_1);
            m_state = WAIT_0;
        }
        else
        {
            m_pSerial->print(m_seq_0);
        }
    }
}

/* 驱动电机发生点击动作 */
void StateMachine::stepperClick(int steps, int pressDelay, int releaseDelay)
{
    m_pStepper->step(-steps);
    delay(pressDelay);
    m_pStepper->step(steps);
    delay(releaseDelay);
}

/* 重置状态机 */
void StateMachine::reset()
{
    m_state = WAIT_0;
}