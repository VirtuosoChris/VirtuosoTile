//  Copyright (c) 2013 Christopher Pugh. All rights reserved.

#ifndef GL_TEXTURE_H_INCLUDED
#define GL_TEXTURE_H_INCLUDED

#include "SharedCounter.h"

#include "GLInclude.h"


//#include <image.h>
#include <stdexcept>///\todo remove

namespace GL
{

struct ImageFormat
{

    int width;
    int height;
    int channels;

    float aspect();

    ImageFormat();

    operator bool()const;

};



class GLTexture: public ImageFormat
{

public:

    Utilities::SharedCounter copyCount;
    GLuint tex;

public:

    operator bool()const;

    void releaseTexture();

    GLTexture();

    GLTexture(const ImageFormat& format, unsigned char* data = NULL);

    //GLTexture(const LDRImage& img);

    void bind()const
    {
        throw std::runtime_error("not implemented");
    }
    
    ~GLTexture();

};

}

#endif

