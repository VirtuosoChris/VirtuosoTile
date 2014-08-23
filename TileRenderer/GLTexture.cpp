//  Copyright (c) 2013 Christopher Pugh. All rights reserved.

#include "GLTexture.h"

using namespace GL;

float ImageFormat::aspect()
{
    return float(width)/float(height);
}

ImageFormat::ImageFormat()
    :width(0), height(0), channels(0)
{

}

ImageFormat::operator bool()const
{
    return width && height && channels;
}



GLTexture::operator bool()const
{
    return tex && (ImageFormat::operator bool());
}


void GLTexture::releaseTexture()
{

    glDeleteTextures(1,&tex);
    tex=0;

}

GLTexture::GLTexture()
    :tex(0)
{


}


GLTexture::GLTexture(const ImageFormat& format, unsigned char* data)
    : ImageFormat(format),
    tex(0)

{

    glGenTextures(1,&tex);

    glBindTexture(GL_TEXTURE_2D, tex);

    const int& n = channels;
    
    GLenum pixformat = n == 4? GL_RGBA :( (n==3)?GL_RGB : ((n==2)?GL_LUMINANCE_ALPHA:GL_LUMINANCE)) ;

 
    glTexImage2D(GL_TEXTURE_2D,0, pixformat, width, height,0,pixformat,GL_UNSIGNED_BYTE,data );

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER, GL_NEAREST);

    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

}



GLTexture::~GLTexture()
{
    if(copyCount.isUnique())
    {
        releaseTexture();
    }
}
