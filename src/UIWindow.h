#pragma once

#include <memory>

#include "UIView.h"
//#include "UIViewPort.h"
#include "GLGui.h"
#include "PlugTypes.h"

#include "ShaderObject.h"
#include "TextEngine.h"

#include <iostream>

//#define GLEW_STATIC
#include "ShaderObject.h"
#include "sparestack.hpp"
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>





using namespace std;

// base class
class Application;
class UIViewPortController;

class UIWindow : public UIView {

private:
    // OPENGL STUFF
    GLfloat aspect_ratio;
    vector<UIView*> vUIViewGlobalStore; // all view including children are stored
        // here so we can grab selection and
        // forward messages accordingly
    vector<int> vUIViewGlobalSpare; // index of spare slots to fill ids

    // each view has in id which is the position in this stack. Views are owned elsewhere (inside other views or in user application)
    sparestack<UIView*> mUiViews;

    int globalViewCount;
    int returnViewCount();
    int selectedViewID;
    

    frustrumStruct frustrum;

    // store a pointer to the app viewport controller. we dont own it so its just a raw pointer
    std::unique_ptr<UIViewPortController> vpCntlr;
    GLGui* eventHandler; // points to already existing handler.

    GLFWwindow* window;
    ShaderObject* windowShader;

    unsigned int VAO;
    unsigned int VBO;
    unsigned int CBO;
    unsigned int SBO;

    GLuint mProgramID; // shader id


public:
    UIWindow(int width, int height, std::string windowTitle = {} );


    ~UIWindow();

    void checkOpenGLError();
    std::vector<UIColour> rectPoints;
    std::vector<UIColour> colours;
    std::vector<UIColour> selectColours;




    void ResizeWindow(int width,
        int height); // GLGui will call this after it handles event.

    float scaleFactor = 1.0f;
    float currentDepth = 0;
    

    void DrawGui();
    void updateGLBuffersAndDraw();

    void drawImmediate();
    void ForceRefresh();
    int id();

    // UIViews are owned at the place where they are created. windows only needs to refer
    void addView(UIView* newView);

    void addSubView(UIView* newView) override;

    //helper for first window id not being set in vpcntlr
    void fixParentRelationShip();



    void deRegisterView(int id); // delete a view.

    // upon construction of UIViews, they will call this static method to get assigned an id and the pointer
    // gets stored in this Application (associated with the 'active' window when the view was added)
    void registerView(UIView* newView);

    UIView* getNodeFromID(int id);

    const char* guiName;
    // void connectNodes(int outputNode_id, int inputNode_id, int fromPlugID,
    //                   int toPlugID); // connect two nodes specifying which nodes
    //                                  // and which inputs....

    void handleEvent(keyStoreStruct key);
    void setHandler(GLGui* eventHandlerPassThrough);

    int nodeIDUnderMousePos(keyStoreStruct key);
    int dragSkip = 0;

    void resetViewport() override;

    GLFWwindow* getWindow();
    TextEngine* textEngine;
    bool programRunning;
    // assigned in application. used to make context current
    int rootID;


    void printShaderLog(GLuint shader) {
        // Make sure name is shader
        if (glIsShader(shader)) {
            // Shader log length
            int infoLogLength = 0;
            int maxLength = infoLogLength;

            // Get info string length
            glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &maxLength);

            // Allocate string
            char* infoLog = new char[maxLength];

            // Get info log
            glGetShaderInfoLog(shader, maxLength, &infoLogLength, infoLog);
            if (infoLogLength > 0) {
                // Print Log
                printf("%s\n", infoLog);
            }
            // Deallocate string
            delete[] infoLog;
        } else {
            printf("Name %d is not a shader\n", shader);
        }
    }

    void CheckGLError(std::string str) {
        GLenum error = glGetError();
        if (error != GL_NO_ERROR) {
            printf("Error! %s %s\n", str.c_str(), gluErrorString(error));
        }
    }

    bool checkShader(GLuint shaderIn, string shaderName) {
        glCompileShader(shaderIn);
        // Check fragment shader for errors
        GLint fShaderCompiled = GL_FALSE;
        glGetShaderiv(shaderIn, GL_COMPILE_STATUS, &fShaderCompiled);
        if (fShaderCompiled != GL_TRUE) {
            cout << "Unable to compile " << shaderName << " shader " << shaderIn
                 << "\n";
            printShaderLog(shaderIn);
            return false;
        }
        CheckGLError("shaders compiled");
    }
};


