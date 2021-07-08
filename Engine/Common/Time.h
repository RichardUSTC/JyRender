// Create: 2021-07-12 9:14.
// Copyright reserved to Li Bin(richardustc@163.com)

#pragma once

#include <chrono>
#include <thread>
#include <chrono>

inline std::chrono::nanoseconds GetNow()
{
    return std::chrono::steady_clock::now().time_since_epoch();
}

inline void SleepFor(std::chrono::nanoseconds duration)
{
    std::this_thread::sleep_for(duration);
}

inline void SleepUntil(std::chrono::time_point<std::chrono::steady_clock> timePoint)
{
    std::this_thread::sleep_until(timePoint);
}
