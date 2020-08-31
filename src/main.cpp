/* A simple program to show how to set up an X window for OpenGL rendering.
 * X86 compilation: gcc -o -L/usr/X11/lib   main main.c -lGL -lX11
 * X64 compilation: gcc -o -L/usr/X11/lib64 main main.c -lGL -lX11
 */
#include <stdio.h>
#include <stdlib.h>
#include "Application.h"
#include "UIView.h"

//#define GLEW_STATIC
#include <GL/glew.h>
//#include <glew.h>
#include <GLFW/glfw3.h>

#include <memory>







int main( int argc, char* argv[] )
  {



    auto application = make_unique<Application>();

    auto view = std::make_unique<UIView>(nullptr, 1024, 768);


    application->exec();
    //mainCore->appLoop();
    //delete viewController;


  return 0;
  }
