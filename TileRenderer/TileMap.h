#ifndef TILE_MAP_H_INCLUDED
#define TILE_MAP_H_INCLUDED

#include "SharedCounter.h"
#include <stdexcept>
#include <iostream>
#include <vector>

#include "GLTexture.h"

class TileMap{
    
    std::vector<GL::GLTexture> indirectionTextures;
    
    public:

        const GL::GLTexture& getIndirectionTexture(unsigned int index)const;
    
        bool validateDimensions()const;
    
        TileMap();
    
        //void insertImage(const LDRImage& img);

        void insertImage(const GL::GLTexture& img);
    
        //TileMap(const LDRImage& img);
    
        TileMap(const GL::GLTexture& tex);
    
        unsigned int bytesPerTile()const;
    
        unsigned int numLayers()const;
    
        std::pair<unsigned int, unsigned int> getDimensions() const;
    
        //structs will get converted to GLubyte pointer before being passed in
        void updateTile(unsigned int x, unsigned int y, GLubyte* data)const;
    
};

#endif