//
//  TileSet.h
//  
//
//  Created by Admin on 7/6/14.
//
//

#include <vector>
#include <string>
#include <map>

#include "GLTexture.h"

class TileSet{
    
    typedef std::vector<GL::GLTexture> TileAtlasArray;
    
    unsigned int tileWidth;
    unsigned int tileHeight;
    
public:

    std::map<std::string, TileAtlasArray> tilesForMaterial; ///\todo was private
    
    const GL::GLTexture& getAtlas(const std::string& material, unsigned int index)const;
    
    unsigned int atlasCount()const;
    
    bool validate()const;
    
    void setTileSize(unsigned int tileW, unsigned int tileH);
    
    void pushAtlasForMaterial(const std::string& mat, const GL::GLTexture& tex);
    
    std::pair<unsigned int, unsigned int>  getTileCount()const;
    std::pair<unsigned int, unsigned int> getTileSize()const;
    
    unsigned int numMaterials()const;
    
    std::string materialNameForIndex(unsigned int idx)const;
    
    
};

