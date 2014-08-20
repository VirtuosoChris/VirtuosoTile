//  Copyright (c) 2013 Christopher Pugh. All rights reserved.

 
#ifndef VIRTUOSO_STATIC_MESH_H
#define VIRTUOSO_STATIC_MESH_H

#include "Mesh.h"

#include "GLShader.h"
#include "SharedCounter.h"

//file executed

namespace Virtuoso{

class GPUMesh
{

public:

    int vertexCount;
    int faceCount;

    //this takes in a mesh object, which has a cpu client copy of the arrays we want to pass to the GPU
    GPUMesh(const Mesh& m)
    {
        initialize(m);
    }

    GPUMesh& operator=(const Mesh& m)
    {
        initialize(m);
        return *this;
    }

    void initialize(const Mesh& in);

    void initialize(std::istream& in);

    GPUMesh() :vertexCount(0), faceCount(0){}

    virtual ~GPUMesh();

    ///issue a draw call for this geometry
    void push();
    
    

protected:

    Utilities::SharedCounter copycount;
    std::vector<GLuint> vbos;
    GLuint indexbuffer;
    std::vector<std::string> attributenames;
    
    std::vector<GLuint> attributecomponents;
    
public:

    
    const std::vector<std::string>& getAttributeNames()const
    {
        return attributenames;
    }

    const std::vector<GLuint>& getVBOHandles()const
    {
        return vbos;
    }

    const GLuint& getIndexBufferHandle()const
    {
        return indexbuffer;
    }

    GLuint& operator[](const std::string& in);

};

}
#endif
