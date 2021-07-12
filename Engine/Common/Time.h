// Create: 2021-07-12 9:14.
// Copyright reserved to Li Bin(richardustc@163.com)

#pragma once

#include <chrono>
#include <thread>
#include <chrono>

using TimePoint = std::chrono::time_point<std::chrono::steady_clock>;
using Duration = std::chrono::nanoseconds;

inline Duration ToDuration(double seconds)
{
    return std::chrono::nanoseconds(static_cast<uint64_t>(seconds * 1e9));
}

inline Duration FrameDuration(uint32_t frameRate)
{
    return std::chrono::nanoseconds(static_cast<uint64_t>(1e9 / frameRate));
}

inline TimePoint GetNow()
{
    return std::chrono::steady_clock::now();
}

inline void SleepFor(Duration duration)
{
    std::this_thread::sleep_for(duration);
}

inline void SleepUntil(TimePoint timePoint)
{
    std::this_thread::sleep_until(timePoint);
}
