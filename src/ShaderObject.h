

#ifndef SHADEROBJECT_H

#define SHADEROBJECT_H

//#include </local1/delliott/glew-1.10.0/include/GL/glew.h>
#include <GL/glew.h>

#include <GLFW/glfw3.h>


#include <cstdio>

#include <string>

#include <iostream>

#include <fstream>

class ShaderObject{







public:



    ShaderObject();

    virtual ~ShaderObject();

    bool loadProgram(const char *vertex_path, const char *fragment_path);

    virtual void freeProgram();

    bool bind();

    void unbind();

    void setTextureUnits();

    GLenum error;

    GLuint myProgramID;
    GLuint getProgramID();

    std::string readFile(const char *filePath);
    void printProgramLog( GLuint program);

    const char *openGLErrorString(GLenum _errorCode);
    void CheckGLError(std::string str);


protected:







private:


    void printShaderLog(GLuint shader);
    void checkOpenGLError();


};


#endif

