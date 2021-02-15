/* A simple program to show how to set up an X window for OpenGL rendering.
 * X86 compilation: gcc -o -L/usr/X11/lib   main main.c -lGL -lX11
 * X64 compilation: gcc -o -L/usr/X11/lib64 main main.c -lGL -lX11
 */
#include "Application.h"
#include "UIView.h"
#include "UIViewPortController.h"
#include <stdio.h>
#include <stdlib.h>

//#define GLEW_STATIC
#include <GL/glew.h>
//#include <glew.h>
#include <GLFW/glfw3.h>

#include <memory>

int main(int argc, char* argv[]) {

    auto application = make_unique<Application>();

    auto view = std::make_unique<UIView>(nullptr, 1024, 512, "firstView");
    view->setExpandable(true);
    view->setColour(0.8, 0.8, 0.8, 1.0);
    //view->setPosition(50, 200);
    //view->setDrawable(false);

    auto vpCntlr = std::make_unique<UIViewPortController>(
        view.get(), HORIZONTAL, "extrablue");
    vpCntlr->setColour(0, 0.2, 0.2, 0.8, 1.0);

    vpCntlr->divide(0.5);
    vpCntlr->setColour(1, 0.1, 0.2, 0.1, 1.0);


    // //auto view2 = std::make_unique<UIView>(nullptr, 1280, 768, "another_window?");

    // ////cout << "about to create 2nd UIVPC" << std::endl;

    // auto testCntlr = std::make_unique<UIViewPortController>(vpCntlr.get(), 0,
    //                                                         VERTICAL, "vpNew");
    // testCntlr->divide(0.5);
    // testCntlr->setColour(0, 0.8, 0.4, 0.3, 1.0);
    // testCntlr->setColour(1, 0.2, 0.2, 0.8, 1.0);

    // // auto window3 = std::make_unique<UIWindow>(2000, 320, "odd one");

    // auto testCntlr2 = std::make_unique<UIViewPortController>(
    //     testCntlr.get(), 1, HORIZONTAL, "vpNew2");
    // testCntlr2->divide(0.4);
    // testCntlr2->setColour(0, 0.4, 0.1, 0.7, 1.0);
    // testCntlr2->setColour(1, 0.1, 0.9, 0.4, 1.0);

    // auto testCntlr3 = std::make_unique<UIViewPortController>(
    //     testCntlr2.get(), 0, VERTICAL, "vpNew3");
    // testCntlr3->divide(0.2);
    // testCntlr3->setColour(0, 0.4, 0.5, 0.2, 1.0);
    // testCntlr3->setColour(1, 0.1, 0.1, 0.4, 1.0);

    // auto testCntlr4 = std::make_unique<UIViewPortController>(
    //     testCntlr3.get(), 0, HORIZONTAL, "vpNew4");
    // testCntlr4->divide(0.3);

    application->exec();
    // mainCore->appLoop();

    return 0;
}
