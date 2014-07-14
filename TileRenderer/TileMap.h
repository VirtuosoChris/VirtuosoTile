#ifndef TILE_MAP_H_INCLUDED
#define TILE_MAP_H_INCLUDED

#include "SharedCounter.h"
#include <stdexcept>
#include <iostream>

class TileMap{
    
        std::vector<GLTexture> indirectionTextures;
    
    public:

        const GLTexture& getIndirectionTexture(unsigned int index)const;
    
        bool validateDimensions()const;
    
        TileMap();
    
        void insertImage(const LDRImage& img);

        void insertImage(const GLTexture& img);
    
        TileMap(const LDRImage& img);
    
        TileMap(const GLTexture& tex);
    
        unsigned int bytesPerTile()const;
    
        unsigned int numLayers()const;
    
        std::pair<unsigned int, unsigned int> dimensions() const;
    
        //structs will get converted to GLubyte pointer before being passed in
        void updateTile(unsigned int x, unsigned int y, GLubyte* data)const;
    
};

#endif