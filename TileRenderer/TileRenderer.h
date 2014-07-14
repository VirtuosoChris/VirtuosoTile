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
    
    Eigen::Matrix3f textureMatrix;
    Eigen::Matrix4f modelviewProjectionMatrix;
    
public:
    
    void update(double timeDelta);
    
    void draw(const TileMap&, const TileSet&)const;
    
    


};

#endif /* defined(____TileRenderer__) */
