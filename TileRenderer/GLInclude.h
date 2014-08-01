//
//  GLInclude.h
//  TileTest
//
//  Created by Admin on 7/31/14.
//  Copyright (c) 2014 Admin. All rights reserved.
//

#ifndef TileTest_GLInclude_h
#define TileTest_GLInclude_h

#ifndef GL_ES_BUILD

#ifndef __APPLE__

#ifdef _WIN32
#include <GL/glew.h>
#endif

#include <GL/GL.h>

#else
#include <OpenGL/gl.h>
#endif


#else
#include <OpenGLES/ES2/gl.h>
#endif

#endif
