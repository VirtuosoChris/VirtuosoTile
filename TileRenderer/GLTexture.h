//  Copyright (c) 2013 Christopher Pugh. All rights reserved.

#ifndef GL_TEXTURE_H_INCLUDED
#define GL_TEXTURE_H_INCLUDED

#include "SharedCounter.h"


#ifndef GL_ES_BUILD
#include <GL/glew.h>
#include <GL/GL.h>
#else
#include <OpenGLES/ES2/gl.h>
#endif

#include <image.h>

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

    GLTexture(const ImageFormat& format);

    GLTexture(const LDRImage& img);

    ~GLTexture();

};


#endif

