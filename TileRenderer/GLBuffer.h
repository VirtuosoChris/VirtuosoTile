//
//  GLBuffer.h
//  TileTest
//
//  Created by Admin on 8/2/14.
//  Copyright (c) 2014 Admin. All rights reserved.
//

#ifndef __TileTest__GLBuffer__
#define __TileTest__GLBuffer__

#include <iostream>
#include "GLInclude.h"
#include "SharedCounter.h"


namespace GL
{
    
    class GLBuffer
    {
        
        GLuint buffer;
        GLenum target;
        
        Utilities::SharedCounter copyCount;
        ///\todo this doesn't handle, copy, then init, then copy.  3 counts, one null instance
        
        std::size_t sizeBytes;
        GLenum usage;
        
        public:
        
        
            inline GLint getHandle()const{return buffer;}
        
            inline GLenum getTarget()const{return target;}
        
            inline std::size_t getBufferSize()const {return sizeBytes;}
        
            inline GLenum getBufferUsage()const{return usage;}
        
        
            inline GLBuffer()
            : buffer(0), target(0), sizeBytes(0), usage(0)
            {
                
            }
        
            GLBuffer(GLenum bTarget, std::size_t sizeBytesIn, const char* dataInit = NULL, GLenum usageIn = GL_STATIC_DRAW);
        
            void initialize(GLenum bTarget, std::size_t sizeBytesIn, const char* dataInit = NULL, GLenum usageIn = GL_STATIC_DRAW);
    
            void freeBuffer();
        
            void subData(std::size_t offset, std::size_t size, const char* data)const;
        
            operator bool()const;
        
            virtual ~GLBuffer();

            void bind()const;
        
        
    };

}



#endif /* defined(__TileTest__GLBuffer__) */
