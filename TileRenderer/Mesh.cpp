//  Copyright (c) 2013 Christopher Pugh. All rights reserved.

#include "Mesh.h"

namespace Virtuoso{

unsigned int Mesh::pointsperface(PrimitiveMode m)
{

    switch(m) {

        //todo: depending on implementation, special kinds of triangles and lines might not be so good for this
    case TRIANGLES:
        return 3;
        break;
    case TRIANGLE_STRIPS:
        return 3;
        break;
    case TRIANGLE_FANS:
        return 3;
        break;
    case LINES:
        return 2;
        break;
    case LINE_LOOP:
        return 2;
        break;
    case POINTS:
        return 1;
        break;
    default:
        return 0;
        break;
    }

}


void Mesh::write(std::ostream& out)const
{

    //write out basic information
    out.write(( char*)&numVerts, sizeof(int)); //how many vertices?
    out.write(( char*)&numFaces, sizeof(int)); //how many faces?
    out.write((char*)&mode, sizeof(int)); //output the primitive type
    out.write((char*)&indexedmesh, sizeof(bool));
    unsigned int numattribs = attributes.size();
    out.write((char*)&(numattribs), sizeof(unsigned int));

    if(indexedmesh) {
        out.write((const char*)&(indexbuffer[0]), sizeof(unsigned int)*indexbuffer.size());
    }

    for(unsigned int i = 0; i < attributes.size(); i++) {
        attributes[i].write(out);
    }

}



}
