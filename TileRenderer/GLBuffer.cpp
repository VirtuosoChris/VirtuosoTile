//
//  GLBuffer.cpp
//  TileTest
//
//  Created by Admin on 8/2/14.
//  Copyright (c) 2014 Admin. All rights reserved.
//

#include "GLBuffer.h"

using namespace GL;

GLBuffer::GLBuffer(GLenum bTarget, std::size_t sizeBytesIn, const char* dataInit, GLenum usageIn)
{
    initialize(bTarget, sizeBytesIn, dataInit, usageIn);
    
}

void GLBuffer::initialize(GLenum bTarget, std::size_t sizeBytesIn, const char* dataInit, GLenum usageIn)
{
    
    
    target = bTarget;
    sizeBytes = sizeBytesIn;
    usage = usageIn;
    
    
    glGenBuffers(1, &buffer);
    glBindBuffer(target, buffer);
    glBufferData(bTarget, sizeBytesIn, dataInit, usageIn);
    
    
    if(bTarget != GL_ARRAY_BUFFER && bTarget != GL_ELEMENT_ARRAY_BUFFER)
    {
        throw std::runtime_error("Invalid buffer target");
    }
    
    bool validUsage = false;
    
    switch(usageIn)
    {
        case GL_STATIC_DRAW:
        case GL_DYNAMIC_DRAW:
        case GL_STREAM_DRAW:
            validUsage=true;
            break;
        default: break;
            
    }
    
    if(!validUsage)
    {
        throw std::runtime_error("Invalid buffer usage target");
    }
    
}


void GLBuffer::freeBuffer(){
    if(buffer)
    {
        glDeleteBuffers(1, &buffer);
        buffer=0;
        sizeBytes=0;
    }
}


void GLBuffer::subData(std::size_t offset, std::size_t size, const char* data)const
{
    bind();///\todo unbinding on every one of these c++ helper classes
    
    glBufferSubData(target,offset,size,data);
}

GLBuffer::operator bool()const
{
    return buffer && glIsBuffer(buffer);
}


GLBuffer::~GLBuffer()
{
    if(copyCount.isUnique()){
        freeBuffer();
    }
}

void GLBuffer::bind()const
{
    glBindBuffer(target, buffer);
}

/* std::shared_ptr<char*> map()
 {
 ///use custom deleter that'll unmap the buffer.  document calling reset() before rendering
 }
 */



