//
//  GLSimpleFramebuffer.cpp
//  Color-Fy
//
//  Created by Chris Pugh on 9/15/14.
//  Copyright (c) 2014 Admin. All rights reserved.
//

#include "GLSimpleFramebuffer.h"
#include "GLTexture.h"

using namespace GL;

void GLSimpleFramebuffer::bind()
{
    glBindFramebuffer(GL_FRAMEBUFFER, fbo);
}

GLSimpleFramebuffer::GLSimpleFramebuffer(ImageFormat format, FramebufferFlags flags)
{
    //get previous framebuffer
    
    glGenFramebuffers(1, & fbo);
    
    //if unable to make fbo?
    
    glBindFramebuffer(GL_FRAMEBUFFER, fbo);
    
    bool hasStencil = flags & STENCIL;
    bool hasDepth = flags & DEPTH;
    bool hasColor = flags & COLOR;
    
    if(hasColor)
    {
        tex = GLTexture(format);
    }
    
    std::size_t depthStencilFlags = (flags & DEPTH) | (flags & STENCIL);

    switch(depthStencilFlags)
    {
        case DEPTH & STENCIL:
            break;
        case DEPTH:
            break;
        case STENCIL:
            throw std::runtime_error("Cannot create framebuffer with stencil but no depth.");
        default:
            throw std::runtime_error("Unrecognized depth-stencil flags");
    }

    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, tex, 0);
    
    glViewport(0,0,format.width,format.height);
    
    unsigned int CLEARFLAGS = 0;
    CLEARFLAGS |= hasDepth ? GL_DEPTH_BUFFER_BIT : 0;
    CLEARFLAGS |= hasStencil ? GL_STENCIL_BUFFER_BIT : 0;
    CLEARFLAGS |= hasColor ? GL_COLOR_BUFFER_BIT : 0;
    
    glClear( CLEARFLAGS);
}

GLSimpleFramebuffer::~GLSimpleFramebuffer()
{
    if(copyCount.isUnique())
    {
        glDeleteFramebuffers(1, & fbo);
    }
}
