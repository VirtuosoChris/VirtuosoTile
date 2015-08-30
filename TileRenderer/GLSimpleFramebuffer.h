//
//  GLSimpleFramebuffer.h
//  Color-Fy
//
//  Created by Chris Pugh on 9/15/14.
//  Copyright (c) 2014 Admin. All rights reserved.
//

#ifndef __Color_Fy__GLSimpleFramebuffer__
#define __Color_Fy__GLSimpleFramebuffer__

#include "GLTexture.h"

namespace GL
{

class GLSimpleFramebuffer
{
public:
    
    enum FramebufferFlags  {DEPTH =1, STENCIL = 1<<1, COLOR = 1<<2};
    GLTexture tex;
    GLTexture depthStencil;
    GLuint fbo;
    
    Utilities::SharedCounter copyCount;
    
    GLSimpleFramebuffer(ImageFormat format, FramebufferFlags flags );
    
    void bind();
    
    //operator bool
    
    virtual ~GLSimpleFramebuffer();

};

}
#endif /* defined(__Color_Fy__GLSimpleFramebuffer__) */
