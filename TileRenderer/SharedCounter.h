//  Copyright (c) 2013 Christopher Pugh. All rights reserved.

#ifndef _SHARED_COUNTER_H_INCLUDED
#define _SHARED_COUNTER_H_INCLUDED

#include <memory>

namespace Utilities
{

///this is a simple utility class which is used for reference counting
///it makes use of a std shared pointer pointing to a dummy location to do the reference counting
///we place an instance of this class as a field in a class which has a shared resource
///when the destructor of that class is called, we can check the reference counter to see if that resource should be freed

class SharedCounter
{

    std::shared_ptr<int> sp;

public:

    SharedCounter():sp(static_cast<int*>(NULL))
    {


    }

    bool isUnique()
    {
        return sp.unique();
    }

    long int count()
    {
        return sp.use_count();
    }

};


}


#endif
