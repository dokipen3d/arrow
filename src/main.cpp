/* A simple program to show how to set up an X window for OpenGL rendering.
 * X86 compilation: gcc -o -L/usr/X11/lib   main main.c -lGL -lX11
 * X64 compilation: gcc -o -L/usr/X11/lib64 main main.c -lGL -lX11
 */
#include <stdio.h>
#include <stdlib.h>
#include "UIViewController.h"

//#define GLEW_STATIC
#include <GL/glew.h>
//#include <glew.h>
#include <GLFW/glfw3.h>

#include <memory>







int main( int argc, char* argv[] )
  {



    //UIViewController *viewController = new UIViewController;
    auto viewController = make_unique<UIViewController>();
    //mainCore->appLoop();
    //delete viewController;


  return 0;
  }
