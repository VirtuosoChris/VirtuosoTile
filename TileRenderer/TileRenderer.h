//
//  TileRenderer.h
//  
//
//  Created by Admin on 7/6/14.
//
//

#ifndef ____TileRenderer__
#define ____TileRenderer__

#include <iostream>


///\todo validate shader material names to show there's no invalid characters

///\todo issue a warning if using a configuration where there are inaccessible data
///this default struct can index an 8192x8192 atlas with 32x32 tiles
struct DefaultTileLayout{

    GLubyte offsetX;
    GLubyte offsetY;
    GLubyte maxAnimationFrame;
    GLubyte repeat : 1;
    GLubyte framerate :7;
    
};

typedef std::pair<std::size_t, std::size_t> BitRange;
typedef std::map<std::string, BitRange> VariableMapping;

/*const char* DefaultTileLayoutFrag =
""
""
""
""
""
""
""
"";
*/

void validateVariableMapping(const VariableMapping& metaData)
{
    
    //check for offsetX
    //check for offsetY
    //check that all min bits are less than max bits
    
}


/*

 xxxx xxxx xxxx xxxx
 xxxx xxxx xxxx xxxx
 xxxx xxxx xxxx xxxx
 xxxx xxxx xxxx xxxx

 */

/*
//inject additional uniforms as well
std::string makeShader(const TileMap& map, const TileSet& tiles, const VariableMapping& metaData ){
    
    bool multipleAtlas = tiles.atlasCount() > 1;

    std::stringstream sstr;
    
    sstr<<"uniform vec2 tileSizeNorm; //tile size normalized to size of atlas \n";
    
    sstr<<"uniform float clock;\n\n";
    
    for(unsigned int i = 0; i < map.numLayers; i++){
        
            sstr<<"uniform sampler2D mapData"<<i<<";\n";

    }
    
    sstr<<'\n';
    
    for(unsigned int i = 0; i < tiles.numMaterials(); i++)
    {
        
        sstr<<"uniform sampler2D "<<materialNameForIndex<<"Tex;\n";
        
    }
    
    sstr<<"varying vec2 texCoord;\n";
    
    //////////////
    
    sstr<<"vec2 atlasCoords;\n"
    
    //calculate tileCoord from texCoord

    sstr<<"vec2 coordOffset = fract(tileCoord);"
    
    
    return sstr.str();
    

}
*/








//draw full screen quad

//transformation matrix for the quad
//transformation matrix for the texture coords

//automatically construct byte array in shader
//add named variable with type, bit range as shader variable
//automatically make material lookup for each texture as rgba


class TileRenderer
{
    

    GLShader program;
    
    double clock;
    
public:
    
    void update(double timeDelta);
    
    void draw(const TileMap&, const TileSet&)const;
    
    


};

#endif /* defined(____TileRenderer__) */
