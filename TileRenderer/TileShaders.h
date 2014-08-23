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

static const char* tileVertexShader =
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

class TileMap;
class TileSet;

template <class LAYOUT>
std::string makeFragmentShader(const TileMap& map, const TileSet& tiles);

std::string makeFragmentShaderHeader(const TileMap& map, const TileSet& tiles);

inline bool validateMaterials(const TileSet& tiles);

std::string makeShaderMaterialLookupsSingleAtlas(  const TileSet& tiles);

template <>
std::string makeFragmentShader<DefaultTileLayout>(const TileMap& map, const TileSet& tiles);



#endif
