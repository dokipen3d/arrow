#include "ShaderObject.h"
#include <iostream>



using namespace std;




ShaderObject::ShaderObject()

{
        cout << "shader object being created" << endl;



}

ShaderObject::~ShaderObject()

{

    freeProgram();

}


bool ShaderObject::loadProgram(const char *vertex_path, const char *fragment_path)

{

    //Success flag

    GLint programSuccess = GL_TRUE;

    std::string vertShaderStr = readFile(vertex_path);
    std::string fragShaderStr = readFile(fragment_path);

    const char *vertShaderSrc = vertShaderStr.c_str();
    const char *fragShaderSrc = fragShaderStr.c_str();




    //Generate program

    //cout << "address of mProg is " << &mProgramID << endl;

    myProgramID = glCreateProgram();

    if (glIsProgram(myProgramID)== GL_TRUE)
    {
        cout << "it IS a program" << endl;

    }


    GLenum error = glGetError();

    if( error != GL_NO_ERROR )

    {

        printf( "Error creating program! %s\n", gluErrorString( error ) );

        printProgramLog( myProgramID );

        return false;

    }
    //cout << mProgramID << endl;
    cout << "got here" << endl;
    //Create vertex shader

    GLuint vertexShader = glCreateShader( GL_VERTEX_SHADER );
    //Get vertex source

    /*
    const GLchar* vertexShaderSource[] = {

    "#version 120\nvoid main(){ gl_Position = ftransform(); }" };
    */

    //Set vertex source

    glShaderSource( vertexShader, 1, &vertShaderSrc, NULL );

    //Compile vertex source

    glCompileShader( vertexShader );

    //Check vertex shader for errors

    GLint vShaderCompiled = GL_FALSE;

    glGetShaderiv( vertexShader, GL_COMPILE_STATUS, &vShaderCompiled );

    if( vShaderCompiled != GL_TRUE )

    {

       printf( "Unable to compile vertex shader %d!\n", vertexShader );

       printShaderLog( vertexShader );

       return false;

    }

    //Attach vertex shader to program

    glAttachShader( myProgramID, vertexShader );


    //Create fragment shader

    GLuint fragmentShader = glCreateShader( GL_FRAGMENT_SHADER );

    //Get fragment source



    //Set fragment source

    glShaderSource( fragmentShader, 1, &fragShaderSrc, NULL );

    //Compile fragment source

    glCompileShader( fragmentShader );

    //Check fragment shader for errors

    GLint fShaderCompiled = GL_FALSE;

    glGetShaderiv( fragmentShader, GL_COMPILE_STATUS, &fShaderCompiled );

    if( fShaderCompiled != GL_TRUE )

    {

    printf( "Unable to compile fragment shader %d!\n", fragmentShader );

    printShaderLog( fragmentShader ); return false;

    }

    //Attach fragment shader to program

    glAttachShader( myProgramID, fragmentShader );

    //Link program

    glLinkProgram( myProgramID );

    //Check for errors

    glGetProgramiv( myProgramID, GL_LINK_STATUS, &programSuccess );

    if( programSuccess != GL_TRUE )

    {

    printf( "Error linking program %d!\n", myProgramID );

    printProgramLog( myProgramID );

    return false;

    }

    return true;


}


void ShaderObject::freeProgram()

{

    glDeleteProgram(myProgramID);

}



bool ShaderObject::bind()

{

    //Use shader
   // cout << "binding shader";
    //if (glIsProgram(mProgramID) != GL_TRUE)
   // {
    //    cout << "something is wrong with program" << endl;
    //}
    glUseProgram( myProgramID );

    //Check for error


    if( glIsProgram(myProgramID) != GL_TRUE )

    {
        GLenum error = glGetError();
    printf( "Error binding shader! %s\n", gluErrorString( error ) );

    printProgramLog( myProgramID );

    return false;

    }



    return true;

}

void ShaderObject::unbind()

{

    //Use default program

    glUseProgram( NULL );

}

GLuint ShaderObject::getProgramID()

{

    return myProgramID;

}





void ShaderObject::printProgramLog( GLuint program)

{

//Make sure name is shader

    if( glIsProgram( program ) )

    {

    //Program log length

    int infoLogLength = 0;

    int maxLength = infoLogLength;

    //Get info string length

    glGetProgramiv( program, GL_INFO_LOG_LENGTH, &maxLength );

    //Allocate string

    char* infoLog = new char[ maxLength ];

    //Get info log

    glGetProgramInfoLog( program, maxLength, &infoLogLength, infoLog );

    if( infoLogLength > 0 )

    {

    //Print Log

    printf( "%s\n", infoLog );

    }

    //Deallocate string

    delete[] infoLog;

    }

    else { printf( "Name %d is not a program\n", program ); }

}



void ShaderObject::printShaderLog(GLuint shader)

{

    //Make sure name is shader

    if( glIsShader( shader ) )

    {

    //Shader log length

    int infoLogLength = 0;

    int maxLength = infoLogLength;

    //Get info string length

    glGetShaderiv( shader, GL_INFO_LOG_LENGTH, &maxLength );

    //Allocate string

    char* infoLog = new char[ maxLength ];

    //Get info log

    glGetShaderInfoLog( shader, maxLength, &infoLogLength, infoLog );

    if( infoLogLength > 0 )

    {

    //Print Log

    printf( "%s\n", infoLog );

    }

    //Deallocate string

    delete[] infoLog;

    }

    else

    {

    printf( "Name %d is not a shader\n", shader );

    }


}


std::string ShaderObject::readFile(const char *filePath) {

    std::string content;

    std::ifstream fileStream(filePath, std::ios::in);



    if(!fileStream.is_open()) {

       std::cerr << "Could not read file " << filePath << ". File does not exist." << std::endl;

       return "";

    }



    std::string line = "";

    while(!fileStream.eof()) {

       std::getline(fileStream, line);

       //content.append(line + "n");

       content.append(line + "\n");

    }



    fileStream.close();

    return content;

}

void ShaderObject::checkOpenGLError()
{
    GLenum error = glGetError();
    if( error != GL_NO_ERROR )
    {
        printf( "Error! %s\n", gluErrorString( error ) );
    }

}
