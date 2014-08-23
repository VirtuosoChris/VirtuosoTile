//
//  GameMap.h
//  TileTest
//
//  Created by Admin on 8/23/14.
//  Copyright (c) 2014 Admin. All rights reserved.
//

#ifndef __TileTest__GameMap__
#define __TileTest__GameMap__

#include <iostream>

#include "TileMap.h"
#include "TileRenderer.h"
#include "TileSet.h"
#include "Timer.h"

struct GameMap
{
    TileSet tiles;
    
    TileMap map;
    
    TileRenderer renderer;
    
    Timer timer;
    
    GameMap(TileSet& tilesIn, TileMap& mapIn);
    
    void draw()const;
    
    void update();
    
};

#endif /* defined(__TileTest__GameMap__) */
