//
//  TileShaders.h
//  
//
//  Created by Admin on 7/13/14.
//
//

#ifndef _TileShaders_h
#define _TileShaders_h


//inject additional uniforms as well
///\todo validate shader material names to show there's no invalid characters

#include "TileLayouts.h"


const char** tileVertexShader =
"uniform mat3 textureMatrix;\n"
"uniform mat4 modelviewProjection;\n"
"\n"
"attribute vec3 position;\n"
"attribute vec2 texcoord;\n"
"\n"
"varying vec2 tc;\n"
"\n"
"void main()\n"
"{\n"
"  vec3 tcmult = textureMatrix * vec3(texCoord, 1.0);\n"
"  tc = tcmult.xy / tcmult.z;\n"
"  gl_Position = modelviewProjection * position;\n"
"\n"
"}\n"
"\n"
"\n"
;


template <class LAYOUT>
std::string makeFragmentShader<LAYOUT>(const TileMap& map, const TileSet& tiles, const VariableMapping& metaData );



std::string makeFragmentShaderHeader(const TileMap& map, const TileSet& tiles, const VariableMapping& metaData )

{


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
    
    
    return sstr.string();


}



template <>
std::string makeFragmentShader<DefaultTileLayout>(const TileMap& map, const TileSet& tiles)
{
    
    //bool multipleAtlas = tiles.atlasCount() > 1;
    
    
    sstr<<"vec2 atlasCoords;\n"
    
    //calculate tileCoord from texCoord
    
    sstr<<"vec2 coordOffset = fract(tileCoord);"
    
    
    return sstr.str();
    
    
}





#endif
