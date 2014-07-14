//
//  TileLayouts.h
//  
//
//  Created by Admin on 7/13/14.
//
//

#ifndef _TileLayouts_h
#define _TileLayouts_h

///\todo issue a warning if using a configuration where there are inaccessible data
///this default struct can index an 8192x8192 atlas with 32x32 tiles
struct DefaultTileLayout{
    
    GLubyte offsetX;
    GLubyte offsetY;
    GLubyte maxAnimationFrame;
    GLubyte repeat : 1;
    GLubyte framerate :7;
    
};


#endif
