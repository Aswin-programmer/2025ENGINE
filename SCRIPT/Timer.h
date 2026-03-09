#include <iostream>
#include <chrono>

class Timer
{
public:
    Timer();

    void Reset();

    double ElapsedSeconds() const;

    double ElapsedMilliseconds() const;
    
private:
    std::chrono::time_point<std::chrono::high_resolution_clock> m_Start;
};