/* A simple program to show how to set up an X window for OpenGL rendering.
 * X86 compilation: gcc -o -L/usr/X11/lib   main main.c -lGL -lX11
 * X64 compilation: gcc -o -L/usr/X11/lib64 main main.c -lGL -lX11
 */
#include <stdio.h>
#include <stdlib.h>
#include "Application.h"
#include "UIView.h"
#include "UIViewPortController.h"

//#define GLEW_STATIC
#include <GL/glew.h>
//#include <glew.h>
#include <GLFW/glfw3.h>

#include <memory>







int main( int argc, char* argv[] )
  {



    auto application = make_unique<Application>();

    auto view = std::make_unique<UIView>(nullptr, 1280, 768, "firstView");
    view->setColour(0.1, 0.7, 0.3, 1.0);
    view->setPosition(500, 500);
    

    auto vpCntlr = std::make_unique<UIViewPortController>(view.get(), 1280, 768, HORIZONTAL, "extrablue");
    vpCntlr->setColour(0, 0.2, 0.2, 0.8, 1.0);
    //cout << "about to reg first UIVPC" << std::endl;

    ////registerView(vpCntlr.get());
    vpCntlr->divide(0.06);

    ////cout << "about to create 2nd UIVPC" << std::endl;

//UIViewPortController* testCntlr = new UIViewPortController(vpCntlr.get(), 1 , VERTICAL, "vpNew");
    //cout << "about to add 2nd UIVPC as sub" << std::endl;

    //vpCntlr->addSubViewPortController(testCntlr, 0);
    //testCntlr->divide(0.1);

    //UIViewPortController* testCntlr2 = new UIViewPortController(vpCntlr.get(), 0.0, 0.0, VERTICAL);
    //testCntlr->addSubViewPortController(testCntlr2, 1);
    //testCntlr2->divide(0.6);

    //UIViewPortController* testCntlr3 = new UIViewPortController(vpCntlr.get(), 0.0, 0.0, HORIZONTAL);
    //testCntlr2->addSubViewPortController(testCntlr3, 1);
    //testCntlr3->divide(0.5);


    application->exec();
    //mainCore->appLoop();


  return 0;
  }
