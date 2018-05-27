#ifndef GOLOVKOV_HW_TIMEUTILS_HPP
#define GOLOVKOV_HW_TIMEUTILS_HPP

#include <chrono>


inline auto getTime() {
    return std::chrono::steady_clock::now();
}

template <class T>
inline auto timeDiff(T lastTimePoint) {
    return std::chrono::duration_cast<std::chrono::nanoseconds >(getTime() - lastTimePoint).count();
}

#endif //GOLOVKOV_HW_TIMEUTILS_HPP
