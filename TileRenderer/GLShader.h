//  Copyright (c) 2013 Christopher Pugh. All rights reserved.

#ifndef _GL_SHADER_H_INCLUDED
#define _GL_SHADER_H_INCLUDED


#ifdef EIGEN
#include <Eigen/Core>
#endif

#include "SharedCounter.h"


#include "GLInclude.h"

///\todo uniform location cache, invalidate functions for it, link method, etc

namespace GL
{

class GLShaderProgram
{

    Utilities::SharedCounter copyCounter;

public:

    GLuint prog;

    void showLinkLog();
    static void showCompilationLog(unsigned int shaderObjectID);

    /* constructors and initializers that take in source for vertex shaders and fragment shaders, and create a GL shader program */

    GLShaderProgram(const char* vertSrc, const char* fragSrc);
    
    GLShaderProgram(const std::string& vertSrc, const std::string& fragSrc);
    
    GLShaderProgram(std::istream& vertStream, std::istream& fragStream);
 
    void initializeShader(const std::string& vertSrc, const std::string& fragSrc);

    void initializeShader(const char* vertSrc, const char* fragSrc);

    void initializeShader(std::istream& vertStream, std::istream& fragStream);
    
    ///dummy constructor
    GLShaderProgram():prog(0) {}

    ///frees up the program object and makes this a "dummy" object
    void releaseProgram();

    ///dtor for a shader program.  releases the program object if the copy count is unique
    virtual ~GLShaderProgram();

    /*functions to set uniform variables*/

    ///uniform float
    void setUniform(const char* name, const float& val);

    ///uniform float2 taking in individual values
    void setUniform(const char* name, const float& val0, const float& val1);

    ///uniform float3 taking individual values
    void setUniform(const char* name, const float& val0, const float& val1, const float& val2);

    void setUniform(const char* name, const float& val0, const float& val1, const float& val2, const float& val3);
 
    ///\todo float array uniform methods
    
#ifdef EIGEN
    ///\todo, would it be worth supporting GLM as an additional ifdef if EIGEN is supported?
    
    ///uniform float3 taking in an eigen vector
    void setUniform(const char* name, const Eigen::Vector3f& val);

    ///uniform float4 taking in an eigen vector
    void setUniform(const char* name, const Eigen::Vector4f& val);

    ///uniform 3x3 matrix taking in an eigen matrix
    void setUniform(const char* name, const Eigen::Matrix3f& val);

    ///uniform 4x4 matrix taking in an eigen matrix
    void setUniform(const char* name, const Eigen::Matrix4f& val);

#endif
    

    ///uniform int
    void setUniform(const char* name, int val);

    ///uniform which binds sampler2D with identifier "name" to the specified texture unit
    void setTexture(const char* name, int unit);

    void setUniform(const char* name, const int& val0, const int& val1);


    void bind();

};

}


#endif
