#pragma once
#include <format>
#include <cstdio>

template<typename ...Args>
void Log(std::format_string<Args...> fmt, Args... args) {
    puts(std::format(fmt, std::forward<Args>(args)...).c_str());
}


