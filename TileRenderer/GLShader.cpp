//  Copyright (c) 2013 Christopher Pugh. All rights reserved.

#include "GLShader.h"
#include <fstream>
#include <iostream>


#ifdef EIGEN
#include <Eigen/Core>
#endif

#include <sstream>

using namespace GL;

GL::GLShaderProgram::GLShaderProgram(const char* vertSrc, const char* fragSrc)
{
    initializeShader(vertSrc, fragSrc);
}


GL::GLShaderProgram::GLShaderProgram(const std::string& vertFile, const std::string& fragFile)
{
    initializeShader(vertFile.c_str(), fragFile.c_str());
}


GL::GLShaderProgram::GLShaderProgram( std::istream& vertStream, std::istream& fragStream)
{
    initializeShader(vertStream, fragStream);
}


void GL::GLShaderProgram::initializeShader(const char* vertCStr, const char* fragCStr)
{
    
    unsigned int vertexShaderObject = glCreateShader(GL_VERTEX_SHADER);
    unsigned int fragmentShaderObject = glCreateShader(GL_FRAGMENT_SHADER);
    
    glShaderSource(vertexShaderObject, 1, (const char**)&vertCStr, NULL);
    glCompileShader(vertexShaderObject);
    
    glShaderSource(fragmentShaderObject, 1, (const char**)&fragCStr, NULL);
    glCompileShader(fragmentShaderObject );
    
    
    // Print error log if compilation failed
    int successVert = 0;
    glGetShaderiv(vertexShaderObject, GL_COMPILE_STATUS, &successVert);
    int successFrag = 0;
    glGetShaderiv(fragmentShaderObject, GL_COMPILE_STATUS, &successFrag);
    
    //std::cout<<"Vertex shader source: "<<vertSrc<<std::endl;
    
    if(!successVert)
    {
        showCompilationLog(vertexShaderObject);
    }
    
    //std::cout<<"Fragment Shader Source: "<<fragSrc<<std::endl;
    if(!successFrag)
    {
        showCompilationLog(fragmentShaderObject);
    }
    
    prog = glCreateProgram();
    glAttachShader(prog, vertexShaderObject);
    glAttachShader(prog, fragmentShaderObject);
    
    
    
    glLinkProgram(prog);
    
    glDeleteShader(fragmentShaderObject);
    glDeleteShader(vertexShaderObject);
    
    int successLink = 0;
    glGetProgramiv(prog, GL_LINK_STATUS, &successLink);
    
    if(!successLink)
    {
        
        showLinkLog();
        
    }
    
}




void GL::GLShaderProgram::initializeShader(std::istream& vertShaderFile, std::istream&  fragShaderFile)
{
    
    ///\todo loadShaderFromFile(string, string)
    /*
    if(!vertShaderFile.is_open())
    {
        throw std::runtime_error("Vertex Shader File Not Open");
    }
    
    if(! fragShaderFile.is_open())
    {
        throw std::runtime_error("Fragment Shader File Not Open");
    }
     */
    
    std::ostringstream s;
    s << vertShaderFile.rdbuf();
    std::string vertSrc=s.str();
    
    std::ostringstream s1;
    s1<<fragShaderFile.rdbuf();
    std::string fragSrc = s1.str();
    
    const char* vertCStr = vertSrc.c_str();
    const char* fragCStr = fragSrc.c_str();
    
    initializeShader(vertCStr, fragCStr);
    
    
}




void GL::GLShaderProgram::initializeShader(const std::string& vertSrc, const std::string& fragSrc)
{
    initializeShader(vertSrc.c_str(), fragSrc.c_str());
}



void GL::GLShaderProgram::showLinkLog()
{

    int infoLogSize = 0;
    glGetProgramiv(prog, GL_INFO_LOG_LENGTH, &infoLogSize);
    char* infoLog = new char[infoLogSize];

    glGetProgramInfoLog(prog, infoLogSize, NULL, infoLog);
    std::cout << infoLog << std::endl;

    delete [] infoLog;

    system("pause");
}


///\todo option for error log other than cout
void GL::GLShaderProgram::showCompilationLog(unsigned int shaderObjectID)
{
    int infoLogSize = 0;
    glGetShaderiv(shaderObjectID, GL_INFO_LOG_LENGTH, &infoLogSize);
    char* infoLog = new char[infoLogSize];

    glGetShaderInfoLog(shaderObjectID, infoLogSize, NULL, infoLog);
    std::cout << "Compiling Shader Error: "<< infoLog << std::endl;

    delete [] infoLog;
    infoLog = 0;

    system("pause");

}




void GL::GLShaderProgram::setTexture(const char* name,  int unit)const
{

    setUniform	(name, unit);


}

void GL::GLShaderProgram::bind()const
{

    glUseProgram(prog);
    
    
}





GL::GLShaderProgram::~GLShaderProgram()
{

    if(copyCounter.isUnique())
    {
        releaseProgram();
    }

}



void GL::GLShaderProgram::releaseProgram()
{

    if(prog)
    {
        glDeleteProgram(prog);
        prog = 0;
    }

}


void GL::GLShaderProgram::setUniform(const char* name, const float& val)const
{
    

    GLint loc = glGetUniformLocation(prog, name);
   

    glUniform1f(loc, val);

}

void GL::GLShaderProgram::setUniform(const char* name, int val)const
{

    GLint loc = glGetUniformLocation(prog, name);

    glUniform1i(loc, val);

}



void GL::GLShaderProgram::setUniform(const char* name, const float& val0, const float& val1)const
{

    GLint loc = glGetUniformLocation(prog, name);

    glUniform2f(loc, val0, val1);


}


void GL::GLShaderProgram::setUniform(const char* name, const int& val0, const int& val1)const
{

    std::cout<<"2i uniform"<<std::endl;
    GLint loc = glGetUniformLocation(prog, name);

    glUniform2i(loc, val0, val1);

}



void GL::GLShaderProgram::setUniform(const char* name, const float& val0, const float& val1, const float& val2)const
{

    GLint loc = glGetUniformLocation(prog, name);

    glUniform3f(loc, val0, val1, val2);

}


void GL::GLShaderProgram::setUniform(const char* name, const float& val0, const float& val1, const float& val2, const float& val3)const
{

    GLint loc = glGetUniformLocation(prog, name);

    glUniform4f(loc, val0, val1, val2,val3);

}


#ifdef EIGEN

void GL::GLShaderProgram::setUniform(const char* name, const Eigen::Vector3f& val)const
{

    GLint loc = glGetUniformLocation(prog, name);

    glUniform3fv(loc, 1, &val[0]);

}




void GL::GLShaderProgram::setUniform(const char* name, const Eigen::Vector4f& val)const
{

    GLint loc = glGetUniformLocation(prog, name);

    glUniform4fv(loc, 1, &val[0]);

}

void GL::GLShaderProgram::setUniform(const char* name, const Eigen::Matrix3f& val)const
{

    GLint loc = glGetUniformLocation(prog, name);

    glUniformMatrix3fv(loc, 1, GL_FALSE, val.data());

}

void GL::GLShaderProgram::setUniform(const char* name, const Eigen::Matrix4f& val)const
{

    GLint loc = glGetUniformLocation(prog, name);

    glUniformMatrix4fv( loc, 1, GL_FALSE, val.data());

}

#endif


