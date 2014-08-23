//
//  Timer.cpp
//  TileTest
//
//  Created by Admin on 8/20/14.
//  Copyright (c) 2014 Admin. All rights reserved.
//

#include <Timer.h>

Timer::Timer()
{
    reset();
}

void Timer::reset()
{
    lastTime = std::chrono::high_resolution_clock::now();
}


double Timer::getDelta()const
{
    
    std::chrono::high_resolution_clock::time_point currentTime =  std::chrono::high_resolution_clock::now();
    
    std::chrono::duration<double> time_span =  std::chrono::duration_cast< std::chrono::duration<double>>(currentTime - lastTime);
    
    return time_span.count();
    
}
