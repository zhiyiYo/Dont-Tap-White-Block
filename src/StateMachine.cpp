#include "StateMachine.h"

/** @brief 创建有限状态机
 * @param stepperNum 步进电机编号，范围 0~3
 * @param pSerial 串口指针
 * @param timeout 重传时间
 */
StateMachine::StateMachine(int stepperNum, Serial *pSerial, int timeout)
{
    if (stepperNum < 0 || stepperNum > 3)
        throw "stepperNum 必须在 0~3 范围内";

    m_stepperNum = stepperNum;
    m_seq_0 = '0' + stepperNum;
    m_seq_1 = '4' + stepperNum;
    m_pSerial = pSerial;
    m_timeout = timeout;
}

/** @brief 状态转移
 * @param msg 输入消息，'\\0' 代表无效指令
 * @param msgType 消息类型，可以是控制指令或者消息回复
 * @return 是否成功转移到下一状态
 */
bool StateMachine::transition(char msg, MessageType msgType)
{
    if (msg == '\0')
        return false;

    bool hasTransition = false;
    switch (m_state)
    {
    case State::WAIT_COMMAND_0:
        if (msg == m_seq_0 && msgType == MessageType::COMMAND)
        {
            // 向串口写入指令并打开计时器
            m_pSerial->writeData(&msg, 1);
            m_timer.start();
            // 状态转移到等待 ACK_1
            m_state = State::WAIT_ACK_0;
            hasTransition = true;
            // printf("发送指令 %c\n", msg);
        }
        break;
    case State::WAIT_ACK_0:
        if (msg == m_seq_0 && msgType == MessageType::ACK)
        {
            // 关闭计时器并转移到等待指令 1 状态
            m_timer.stop();
            m_state = State::WAIT_COMMAND_1;
            hasTransition = true;
            // printf("收到回复 %c\n", msg);
        }
        else if (isTimerOverflow(m_timeout))
        {
            // 定时器溢出就重传指令
            m_pSerial->writeData(&m_seq_0, 1);
            m_timer.restart();
            printf("重传指令 %c\n", msg);
        }
        break;
    case State::WAIT_COMMAND_1:
        if (msg == m_seq_1 && msgType == MessageType::COMMAND)
        {
            m_pSerial->writeData(&msg, 1);
            m_timer.start();
            m_state = State::WAIT_ACK_1;
            hasTransition = true;
            // printf("发送指令 %c\n", msg);
        }
        break;
    case State::WAIT_ACK_1:
        if (msg == m_seq_1 && msgType == MessageType::ACK)
        {
            m_timer.stop();
            m_state = State::WAIT_COMMAND_0;
            hasTransition = true;
            // printf("收到回复 %c\n", msg);
        }
        else if (isTimerOverflow(m_timeout))
        {
            m_pSerial->writeData(&m_seq_1, 1);
            m_timer.restart();
            printf("重传指令 %c\n", msg);
        }
        break;
    default:
        break;
    }

    return hasTransition;
}

/** @brief 定时器是否溢出
 * @param maxMilliseconds 计时器最大毫秒数
 */
bool StateMachine::isTimerOverflow(int64_t maxMilliseconds)
{
    if (!m_timer.isRunning())
        return false;

    int64_t duration = m_timer.getDuration();
    return duration > maxMilliseconds;
}

/* 重置状态机 */
void StateMachine::reset()
{
    m_timer.stop();
    m_state = State::WAIT_COMMAND_0;
    // 重置 Arduino 上的状态机
    char resetCommand = 'r';
    m_pSerial->writeData(&resetCommand, 1);
}