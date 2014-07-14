//  Copyright (c) 2013 Christopher Pugh. All rights reserved.

#ifndef _GL_SHADER_H_INCLUDED
#define _GL_SHADER_H_INCLUDED

#include <Eigen/Core>
#include <SharedCounter.h>

#ifndef GL_ES_BUILD
#include <GL/glew.h>
#else
#include <OpenGLES/ES2/gl.h>
#endif


namespace GL
{

class GLShaderProgram
{

    Utilities::SharedCounter copyCounter;

public:

    GLuint prog;

    void showLinkLog();
    static void showCompilationLog(unsigned int shaderObjectID);

    /* constructors that take in file names for vertex shaders and fragment shaders*/

    GLShaderProgram(const char* vertFile, const char* fragFile);
    
    GLShaderProgram(const std::string& vertFile, const std::string& fragFile);
    
    GLShaderProgram(std::istream& vertStream, std::istream& fragStream);
 
    void initializeShader(const std::string& vertFile, const std::string& fragFile);

    void initializeShader(const char* vertFile, const char* fragFile);

    ///dummy constructor
    GLShader():prog(0) {}

    ///frees up the program object and makes this a "dummy" object
    void releaseProgram();

    ///dtor for a shader program.  releases the program object if the copy count is unique
    virtual ~GLShader();

    /*functions to set uniform variables*/

    ///uniform float
    void setUniform(const char* name, const float& val);

    ///uniform float2 taking in individual values
    void setUniform(const char* name, const float& val0, const float& val1);

    ///uniform float3 taking individual values
    void setUniform(const char* name, const float& val0, const float& val1, const float& val2);

    void setUniform(const char* name, const float& val0, const float& val1, const float& val2, const float& val3);

    ///uniform float3 taking in an eigen vector
    void setUniform(const char* name, const Eigen::Vector3f& val);

    ///uniform float4 taking in an eigen vector
    void setUniform(const char* name, const Eigen::Vector4f& val);

    ///uniform 3x3 matrix taking in an eigen matrix
    void setUniform(const char* name, const Eigen::Matrix3f& val);

    ///uniform 4x4 matrix taking in an eigen matrix
    void setUniform(const char* name, const Eigen::Matrix4f& val);

    ///uniform int
    void setUniform(const char* name, int val);

    ///uniform which binds sampler2D with identifier "name" to the specified texture unit
    void setTexture(const char* name, int unit);

    void setUniform(const char* name, const int& val0, const int& val1);


    void bind();

};

}


#endif
