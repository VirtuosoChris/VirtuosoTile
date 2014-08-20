//
//  SimpleImage.cpp
//  TileTest
//
//  Created by Admin on 8/19/14.
//  Copyright (c) 2014 Admin. All rights reserved.
//

#include "SimpleImage.h"
#include "stb_image.h"


SimpleImage loadImageFromFile(const char* filename, int forceChannels)
{
    
    
    SimpleImage img;
    
    
    img.data.reset( stbi_load(filename, &img.width, &img.height, &img.channels, forceChannels), stbi_image_free );
    
    
    return img;
    
}

