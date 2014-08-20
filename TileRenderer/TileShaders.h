//
//  TileShaders.h
//  
//
//  Created by Admin on 7/13/14.
//
//

#ifndef _TileShaders_h
#define _TileShaders_h


///\todo validate shader material names to show there's no invalid characters

#include "TileLayouts.h"
#include <sstream>

const char* tileVertexShader =
"uniform mat3 textureMatrix;\n"
"uniform mat4 modelviewProjection;\n"
"uniform vec2 mapSize;\n"
"\n"
"attribute vec3 position;\n"
"attribute vec2 texcoord;\n"
"\n"
"varying vec2 tileCoord;\n"
"varying vec2 tc;\n"
"\n"
"void main()\n"
"{\n"
"  vec3 tcmult = textureMatrix * vec3(texcoord, 1.0);\n"
"  tc = tcmult.xy / tcmult.z;\n"
"  tileCoord = mapSize * tc;\n"
"  gl_Position = modelviewProjection * vec4(position,1.0);\n"
"\n"
"}\n"
"\n"
"\n"
;


template <class LAYOUT>
std::string makeFragmentShader(const TileMap& map, const TileSet& tiles);



std::string makeFragmentShaderHeader(const TileMap& map, const TileSet& tiles)

{

    std::stringstream sstr;
    
    sstr<<"uniform vec2 tileSizeNorm;\n";
    sstr<<"\nuniform float clock;\n\n";
    
    for(unsigned int i = 0; i < map.numLayers(); i++){
        
        sstr<<"uniform sampler2D mapData"<<i<<";\n";
        
    }
    
    sstr<<'\n';
    
    for(unsigned int i = 0; i < tiles.numMaterials(); i++)
    {
        
        sstr<<"uniform sampler2D "<<tiles.materialNameForIndex(i)<<"_Tex;\n";
        
    }
    
    sstr<<"varying vec2 tc;\n";
    sstr<<"varying vec2 tileCoord;\n"; //the texture coordinate is a tile index
    
    
    sstr<<"\n\nvoid main(){\n";
    
    return sstr.str();


}


bool validateMaterials(const TileSet& tiles)
{
//should validate that all the material names are valid for glsl
    
    return true;
    
/*    for(unsigned int i = 0; i < tiles.numMaterials(); i++)
    {
        ///\todo fill this in

    }
  */

}


std::string makeShaderMaterialLookupsSingleAtlas(  const TileSet& tiles)
{
    
    
    std::stringstream sstr;
    
    
    ///\todo tilesForMaterial should be private
    for(auto it = tiles.tilesForMaterial.begin(); it != tiles.tilesForMaterial.end(); it++)
    {
        
        if(it->second.size()>1)
        {
            throw std::runtime_error("Shader being generated doesn't handle multiple atlases per material");
        }
        
        const std::string& materialName = it->first;
        
        sstr<<"vec4 "<<materialName<<"_lookup = texture2D("<<materialName<<"_Tex, atlasCoords);\n";
        
        
    }
    
    
    
    return sstr.str();


}


template <>
std::string makeFragmentShader<DefaultTileLayout>(const TileMap& map, const TileSet& tiles)
{
    
    if(!validateMaterials(tiles))
    {
        ///\todo append name of material here & error
        throw std::runtime_error("Material name contains invalid characters");
        
    }
    
    std::stringstream sstr;
    
    sstr<<makeFragmentShaderHeader(map, tiles);
   
    sstr<<"vec2 coordOffset = fract(tileCoord);\n";
    
    sstr<<"vec4 lookup_mapData0 = texture2D(mapData0, tc);\n";
   
    sstr<<"float offsetX = 255.0 * lookup_mapData0.r;\n";
    sstr<<"float offsetY = 255.0 * lookup_mapData0.g;\n";
    sstr<<"float maxAnimationFrame = 255.0 * lookup_mapData0.b;\n";
    sstr<<"\nint tmp = (int(lookup_mapData0.a * 255.0) / 128);\n";
    sstr<<"bool repeat = tmp > 0;\n";
    sstr<<"int framerate = int(lookup_mapData0.a * 255.0) - (tmp * 128);\n\n";
    
    //generate code to make tile lookup coords
    
    sstr<<"vec2 atlasCoords = (coordOffset + vec2(offsetX, offsetY) ) * tileSizeNorm ;\n";
    
    sstr<<makeShaderMaterialLookupsSingleAtlas(  tiles);
    
    
    ///\todo usage should be a function call
    
    sstr<<"\ngl_FragColor= default_lookup;\n"<<std::endl;
    
    
    sstr<<"}";
    
    return sstr.str();
    
    
}





#endif
