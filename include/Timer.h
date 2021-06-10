#pragma once
#include <chrono>

class Timer
{
public:
    Timer() {}
    int64_t getDuration();
    void start();
    void stop();
    bool isRunning() const;

private:
    bool m_isRunning = false;
    std::chrono::time_point<std::chrono::high_resolution_clock> m_start;
};