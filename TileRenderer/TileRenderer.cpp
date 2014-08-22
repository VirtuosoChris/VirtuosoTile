//
//  TileRenderer.cpp
//  
//
//  Created by Admin on 7/6/14.
//
//

#include "TileRenderer.h"
#include "TileMap.h"
#include "TileSet.h"
#include "Quad.h"
#include "GPUMesh.h"


///replace with something better that doesn't need a mountain of deprecated base code
static void screenQuad()
{
    static Virtuoso::Quad q;
    static Virtuoso::GPUMesh quad(q);
    
    quad.push();
    
}


void TileRenderer::update(double timeDelta)
{
    
    clock += timeDelta;

}


void TileRenderer::draw(const TileMap& map, const TileSet& tiles)const
{
    
    //-----validate map and tile data structures prior to drawing-----
    
    map.validateDimensions();
    tiles.validate();
    
    program.bind();
    
    //----set vertex shader uniforms -----
    
    program.setUniform("textureMatrix", textureMatrix);
    program.setUniform("modelviewProjection", modelviewProjectionMatrix);

    auto dims = map.getDimensions();
    program.setUniform("mapSize", (float)dims.first, (float)dims.second );
    
    //-----set fragment shader uniforms------
    //uniform sampler2D mapData0;
    //uniform sampler2D default_Tex;
    
    //tileSizeNorm is size of tile in atlas, normalized
    auto tilesAxis = tiles.getTileCount();
    program.setUniform("tileSizeNorm", 1.0f / tilesAxis.first, 1.0f / tilesAxis.second);
    
    program.setUniform("clock", (float)clock);
    
    screenQuad();
    
}


TileRenderer::TileRenderer()
: clock(0.0),
textureMatrix(Eigen::Matrix3f::Identity()),
modelviewProjectionMatrix(Eigen::Matrix4f::Identity())
{
    
    
}

