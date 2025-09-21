#pragma once
#include <concepts>
#include <chrono>

namespace gtime {
    using Point = std::chrono::steady_clock::time_point;

    template<typename T = float, typename Rep = std::milli>
    using Duration = std::chrono::duration<T, Rep>;

    Point Now();

    class Timer {
    public:
        Timer();
        void Reset();
        template<std::floating_point T = float, typename Rep = std::milli>
        T GetTime() {
            Point now = Now();
            return (Duration<T, Rep>(now - m_start)).count();
        }
    private:
        Point m_start;
    };
}

