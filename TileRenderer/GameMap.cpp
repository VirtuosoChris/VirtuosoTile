//
//  GameMap.cpp
//  TileTest
//
//  Created by Admin on 8/23/14.
//  Copyright (c) 2014 Admin. All rights reserved.
//

#include "GameMap.h"
#include "TileShaders.h"

static void bindLocs(GL::GLShaderProgram& shader)
{
    glBindAttribLocation(shader.prog,0,"position");
    
    glBindAttribLocation(shader.prog,1,"texcoord");
    
    glLinkProgram(shader.prog);
}


GameMap::GameMap(TileSet& tilesIn, TileMap& mapIn)
    :tiles(tilesIn),
    map(mapIn)
{
    std::string shaderSrc = makeFragmentShader<DefaultTileLayout>(map, tiles);
        
    renderer.program.initializeShader(tileVertexShader, shaderSrc.c_str());
    
    std::cout<<"VERTEX SHADER\n---------\n"<<tileVertexShader<<std::endl;
    std::cout<<"\nFRAGMENT SHADER\n--------\n"<<shaderSrc.c_str()<<std::endl;
        
    bindLocs(renderer.program);
        
}

void GameMap::update()
{
      renderer.update( timer.getDelta() );
}
    
void GameMap::draw()const
{
    renderer.draw(map, tiles);
}
