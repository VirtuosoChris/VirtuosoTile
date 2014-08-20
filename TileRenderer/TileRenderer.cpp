//
//  TileRenderer.cpp
//  
//
//  Created by Admin on 7/6/14.
//
//

#include "TileRenderer.h"


void TileRenderer::update(double timeDelta)
{
    
    clock += timeDelta;

}


void TileRenderer::draw(const TileMap& map, const TileSet& tiles)const
{
    program.bind();
    
    program.setUniform("textureMatrix", textureMatrix);
    
    
    
}


TileRenderer::TileRenderer()
: clock(0.0),
textureMatrix(Eigen::Matrix3f::Identity()),
modelviewProjectionMatrix(Eigen::Matrix4f::Identity())
{
    
    
}

