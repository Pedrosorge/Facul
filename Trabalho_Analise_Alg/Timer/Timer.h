#ifndef TIMER
#define TIMER

#include <chrono>

class Timer{

    private:
        std::chrono::_V2::system_clock::time_point start;

    public: 

        void startTimer();
        long long endTimer();

};

#endif