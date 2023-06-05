#pragma once

#include <chrono>

#include <iostream>

using namespace std::chrono_literals;

using Clock = std::chrono::steady_clock;
using Timepoint = Clock::time_point;
using Duration = Clock::duration;

inline std::ostream &operator<<(std::ostream &str, Duration dur ) {
    return str<<dur.count();
}
