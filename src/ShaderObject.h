

#ifndef SHADEROBJECT_H

#define SHADEROBJECT_H

//#include </local1/delliott/glew-1.10.0/include/GL/glew.h>
#include <GL/glew.h>

#include "/local1/delliott/glfw-3.0.3/include/GLFW/glfw3.h"


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


    GLuint myProgramID;
    GLuint getProgramID();

    std::string readFile(const char *filePath);
    void printProgramLog( GLuint program);



protected:







private:


    void printShaderLog(GLuint shader);
    void checkOpenGLError();


};


#endif

