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
    
    if(!map.validateDimensions())
    {
        throw std::runtime_error("Validating dimensions failed");
    }
    else if(!tiles.validate())
    {
        throw std::runtime_error("Validating tiles failed");
    }
    
    program.bind();
    
    //----set vertex shader uniforms -----
    
    program.setUniform("textureMatrix", textureMatrix);
    program.setUniform("modelviewProjection", modelviewProjectionMatrix);

    auto dims = map.getDimensions();
    program.setUniform("mapSize", (float)dims.first, (float)dims.second );
    
    
    //-----set fragment shader uniforms------
  
    auto tilesAxis = tiles.getTileCount();
    program.setUniform("tileSizeNorm", 1.0f / tilesAxis.first, 1.0f / tilesAxis.second);
    
    program.setUniform("clock", (float)clock);
    
    GLint tex = 0;
    
    //go through each material, bind the atlas to an active texture, and set the uniform to the unit index
    for(auto it = tiles.tilesForMaterial.begin(); it != tiles.tilesForMaterial.end(); it++)
    {
        if(it->second.size()>1)
        {
            throw std::runtime_error("Shader doesn't handle multiple atlases per material");
        }
        
        const std::string& materialName = it->first + "_Tex";
        
        glActiveTexture(GL_TEXTURE0 + (tex));
        
        glBindTexture(GL_TEXTURE_2D, tiles.getAtlas(it->first, 0).tex);
        
        program.setTexture(materialName.c_str(), tex);
        
        tex++;
        
    }
    
    //go through all the map layers, bind to a texture, set uniform to unit index
    for(unsigned int i = 0; i < map.numLayers(); i++){
        
        std::stringstream sstr;
        
        sstr<<"mapData"<<i;
        
        glActiveTexture(GL_TEXTURE0 + (tex));
        
        GLint handle = map.getIndirectionTexture(i).tex;
        
        glBindTexture(GL_TEXTURE_2D, handle);
        
        program.setTexture(sstr.str().c_str(), tex);
        
        tex++;
        
    }
    
    //-------draw------
    screenQuad();


    
}


TileRenderer::TileRenderer()
: clock(0.0),
textureMatrix(Eigen::Matrix3f::Identity()),
modelviewProjectionMatrix(Eigen::Matrix4f::Identity())
{
    
    
}

