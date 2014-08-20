//  Copyright (c) 2013 Christopher Pugh. All rights reserved.

#ifndef QUAD_H_INCLUDED
#define QUAD_H_INCLUDED


#include "Mesh.h"

namespace Virtuoso{

///Constructs a triangle mesh with positions, normals, and texture coordinates.
class Quad : public Mesh{

 public:

    ///Generate a screen space quad spanning -1 to +1 on the xy axis (clip space in OpenGL)
    ///z = 0 for all vertices
    Quad();

    ///Generate a quad given the min and max corners in the x-y plane.
    ///z = 0 for all vertices
    Quad(float minX, float minY, float maxX, float maxY);

};

}
#endif // QUAD_H_INCLUDED
