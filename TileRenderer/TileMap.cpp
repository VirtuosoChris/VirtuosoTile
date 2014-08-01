//
//  TileMap.cpp
//  
//
//  Created by Admin on 7/6/14.
//
//

#include "TileMap.h"

using namespace GL;

const GLTexture& TileMap::getIndirectionTexture(unsigned int index)const
{
    
    if(index >= indirectionTextures.size())
    {
        throw std::runtime_error("getIndirectionTexture out of bounds for TileMap");
    }
    
    return indirectionTextures[index];

}


void TileMap::updateTile(unsigned int x, unsigned int y, GLubyte* data)const
{
    
    if(!validateDimensions())
    {
        throw std::runtime_error("TileMap validation failed in updateTile");
    }
    
    if(!indirectionTextures.size())
    {
        throw std::runtime_error("Called updateTile on empty TileMap");
    }
    
    auto w = indirectionTextures[0].width;
    auto h = indirectionTextures[0].height;
    
    if((!(x < w)) || (!(y < h)))
    {
        throw std::runtime_error("TileMap update out of bounds");
    }
    
    
    unsigned int offsetBytes = 0;
    for(unsigned int i =0; i < indirectionTextures.size(); i++)
    {
        
        indirectionTextures[i].bind();
        
        auto channels = indirectionTextures[i].channels;
        
        unsigned int bytesThisImg = channels;
        
        GLenum format[] = {GL_LUMINANCE, GL_LUMINANCE_ALPHA, GL_RGB, GL_RGBA};
        
        glTexSubImage2D(GL_TEXTURE_2D, 0, x,y,1,1, format[channels],GL_UNSIGNED_BYTE,&data[offsetBytes]);
        
        offsetBytes += bytesThisImg;
        
    
    }
    

    
}

bool TileMap::validateDimensions()const
{
    
    if(!indirectionTextures.size())
    {
        return true;
    }
    
    unsigned int width= indirectionTextures[0].width;
    unsigned int height = indirectionTextures[0].height;
    
    for(unsigned int i = 1; i < indirectionTextures.size(); i++)
    {
        if(indirectionTextures[i].width != width)return false;
        if(indirectionTextures[i].height != height)return false;
    }
    return true;
}


std::pair<unsigned int, unsigned int> TileMap::dimensions() const
{

    if(indirectionTextures.size())
    {

        const GLTexture& tex = indirectionTextures[0];
        
        return std::pair<unsigned int, unsigned int>(tex.width, tex.height);

    }
    
    return std::pair<unsigned int, unsigned int>(0u,0u);
    
}


TileMap::TileMap()
{
    
}

/*
void TileMap::insertImage(const LDRImage& img)
{
    indirectionTextures.push_back(GLTexture(img));
    
}


TileMap::TileMap(const LDRImage& img){
    
    insertImage(img);
    
}

*/

void TileMap::insertImage(const GLTexture& img)
{
    indirectionTextures.push_back(img);

}


TileMap::TileMap(const GLTexture& tex){
    
    insertImage(tex);
    
}


unsigned int TileMap::bytesPerTile()const
{
    unsigned int rval;
    for(unsigned int i =0 ; i < indirectionTextures.size(); i++)
    {
        rval += indirectionTextures[i].channels;
        
    }
    return rval;
    
}

unsigned int TileMap::numLayers()const{
    return (unsigned int)indirectionTextures.size();
}




