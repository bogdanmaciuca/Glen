#include "gtime.h"

namespace gtime {
    Point Now() {
        return std::chrono::steady_clock::now();
    };

    Timer::Timer() {
        m_start = Now();
    }
    void Timer::Reset() {
        m_start = Now();
    }
}

