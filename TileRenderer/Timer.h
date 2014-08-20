//  Copyright (c) 2013 Christopher Pugh. All rights reserved.

#ifndef VIRTUOSO_TIMER_H_INCLUDED
#define VIRTUOSO_TIMER_H_INCLUDED

#include <chrono>

class Timer{

public:

    std::chrono::high_resolution_clock::time_point lastTime;

    Timer();

    void reset();

    double getDelta();
    
};

#endif
