//
//  SimpleImage.h
//  TileTest
//
//  Created by Admin on 8/19/14.
//  Copyright (c) 2014 Admin. All rights reserved.
//

#ifndef TileTest_SimpleImage_h
#define TileTest_SimpleImage_h

#include <memory>

struct SimpleImage
{
    
    int width;
    int height;
    int channels;
    
    std::shared_ptr<unsigned char> data;
    

};



SimpleImage loadImageFromFile(const char* filename, int forceChannels=0);




#endif
