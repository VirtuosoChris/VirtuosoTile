//
//  TileLayouts.h
//  
//
//  Created by Admin on 7/13/14.
//
//

#ifndef _TileLayouts_h
#define _TileLayouts_h
#include <cstdint>

///\todo issue a warning if using a configuration where there are inaccessible data
///this default struct can index an 8192x8192 atlas with 32x32 tiles
struct DefaultTileLayout{
    
    std::uint8_t offsetX;
    std::uint8_t  offsetY;
    std::uint8_t  maxAnimationFrame;
    std::uint8_t  repeat : 1;
    std::uint8_t  framerate :7;
    
};


#endif
