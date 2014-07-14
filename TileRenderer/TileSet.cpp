#include "TileSet.h"

std::pair<unsigned int, unsigned int> TileSet::getTileDimensions()const{
    
    return std::pair<unsigned int, unsigned int>(tileWidth, tileHeight);
    
}



unsigned int TileSet::numMaterials()const
{
    return tilesForMaterial.size();
    
}

std::string TileSet::materialNameForIndex(unsigned int idx)const
{
    
    if(idx >= tilesForMaterial.size())
    {
        throw std::runtime_error("TileSet materialNameForIndex out of bounds");
    }
    
    unsigned int ptr =0;
    
    for(auto it = tilesForMaterial.begin(); it != tilesForMaterial.end(); it++)
    {
        if(ptr==idx){
        
            return it->first;
        }
        
        ptr++;
    
    }
    
    
}


unsigned int TileSet::atlasCount()const{
    
    if(!tilesForMaterial.size()){
        return 0;
    }
    
    return tilesForMaterial.begin()->second.size();
    
}


const GLTexture& TileSet::getAtlas(std::string& material, unsigned int index)const{
    
    TileAtlasArray& arr = tilesForMaterial[material];
    
    if(index >= arr.size())
    {
        throw std::runtime_error("Out of bounds index for TileSet texture atlas getter");
    }
    
    return arr[index];
    
}


void TileSet::pushAtlasForMaterial(std::string mat, const GLTexture& tex)
{

    tilesForMaterial[mat].push_back(tex);
    
    
}


void TileSet::setTileSize(unsigned int tileW, unsigned int tileH)
{
    
    tileWidth = tileW;
    tileHeight = tileH;
    
}


bool TileSet::validate()const
{
    
    //material empty set
    if(!tilesForMaterial.size())return true;
    
    auto it = tilesForMaterial.begin();
    
    unsigned int images = it->second.size();
    
    unsigned int imageWidth=0;
    unsigned int imageHeight=0;
    
    
    if(it->second.size())
    {
        
        imageWidth = it->second[0].width;
        imageHeight = it->second[0].height;
        
        //make sure that the atlases are aligned to tile sizes
        if((imageWidth % tileWidth))return false;
        if((imageHeight % tileHeight))return false;
        
    }
    
    for(unsigned int i = 1; i < it->second.size(); i++)
    {
        //make sure every image for every material is the same size
        if(it->second[i].width != imageWidth)return false;
        if(it->second[i].height != imageHeight)return false;
        
    }
    
    it++;
    
    for( ;it != tilesForMaterial.end(); it++)
    {
        
        //make sure every material has the same number of images
        if(it->second.size() != images)
        {
            return false;
        }
        
        for(unsigned int i = 0; i < it->second.size(); i++){
            //make sure every image for every material is the same size
            if(it->second[i].width != imageWidth)return false;
            if(it->second[i].height != imageHeight)return false;
            
        }
        
        
    }
    
    
    
    return true;
}


