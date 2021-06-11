#include "Timer.h"
using std::chrono::high_resolution_clock;

/* 打开计时器 */
void Timer::start()
{
    if (m_isRunning)
        return;
    m_isRunning = true;
    m_start = high_resolution_clock::now();
}

/* 停止计时器 */
void Timer::stop()
{
    m_isRunning = false;
}

/* 重新打开计时器 */
void Timer::restart()
{
    stop();
    start();
}

/* 判断计时器是否正在运行 */
bool Timer::isRunning() const
{
    return m_isRunning;
}

/* 返回自计时器打开之后到现在的时间间隔，以毫秒计，如果计时器没在运行，返回 0 */
int64_t Timer::getDuration()
{
    if (!m_isRunning)
        return 0;
    auto now = high_resolution_clock::now();
    int64_t duration = std::chrono::duration_cast<std::chrono::milliseconds>(now - m_start).count();
    return duration;
}