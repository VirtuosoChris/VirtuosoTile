//
//  TileSet.h
//  
//
//  Created by Admin on 7/6/14.
//
//

class TileSet{
    
    typedef std::vector<GLTexture> TileAtlasArray;
    
    std::map<std::string, TileAtlasArray> tilesForMaterial;
    
    unsigned int tileWidth;
    unsigned int tileHeight;
    
public:
	   
    const GLTexture& getAtlas(std::string& material, unsigned int index)const;
    
    unsigned int atlasCount()const;
    
    bool validate()const;
    
    void setTileSize(unsigned int tileW, unsigned int tileH);
    
    void pushAtlasForMaterial(std::string mat, const GLTexture& tex);
    
    std::pair<unsigned int, unsigned int> getTileDimensions()const;
    
    unsigned int numMaterials()const;
    
    std::string materialNameForIndex(unsigned int idx)const;
    
    
};

