#include "Timer.h"

void Timer::startTimer(){
    start = std::chrono::high_resolution_clock::now();
};

long long Timer::endTimer(){
    return std::chrono::duration_cast<std::chrono::milliseconds> (std::chrono::high_resolution_clock::now() - start).count();
};


