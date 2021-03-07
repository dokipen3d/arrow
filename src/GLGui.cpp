#include "GLGui.h"
#include <string.h>

#include "Application.h"
#include "Node.h"
#include "UIWindow.h"
#include <algorithm>
#include <cmath>
#include <iostream>
#include <stdio.h>

// class AppCore;

keyStoreStruct GLGui::keyStore;
UIRect GLGui::windowRect;
int globalViewCount;
bool GLGui::windowResized;
bool GLGui::firstWindowCreated(false);
GLFWwindow* GLGui::currentEventWindow;
std::chrono::steady_clock::time_point GLGui::last;
bool GLGui::ready(true);

GLGui::GLGui() {

    if (glfwInit() != GL_TRUE) {
        CloseGUI(1);
    }
    windowResized = false;

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, 1);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwSwapInterval(0);
}

GLGui::~GLGui() {
}
void GLGui::createWindow(std::size_t id, int width, int height,
                         std::string title) {

    auto window = glfwCreateWindow(width, height, title.c_str(), NULL, NULL);
    auto localid = glfwWindows.push(window);

    if (!window) {
        glfwTerminate();
        cout << "NOOOOOO!" << endl;
        exit(EXIT_FAILURE);
    }

    if (!firstWindowCreated) {
        glfwMakeContextCurrent(
            window); // important so that glew init doesn't fail

        glewExperimental =
            GL_TRUE; // so that glew can getStringi and retrieve pointers
        GLenum err = glewInit();
        if (err != GLEW_OK) {
            // Problem: glewInit failed, something is seriously wrong.
            cout << "glewInit failed, aborting." << endl;
        }
        glGetError(); // clear error
        firstWindowCreated = true;
        currentEventWindow = window;
    }
    // ids should be synced but not incredibly important. the ids should be
    // dicated by the creation of UIWindows in the user code
    assert(localid == id);

    glfwSetWindowUserPointer(window, reinterpret_cast<void*>(id));
    glfwSetKeyCallback(window, GLGui::keyCallback);
    glfwSetMouseButtonCallback(window, GLGui::MouseButtonCallback);
    glfwSetCursorPosCallback(window, GLGui::MousePosCallback);
    glfwSetWindowSizeCallback(window, GLGui::framebuffer_size_callback);
    glfwSetWindowRefreshCallback(window, GLGui::window_refresh_callback);
}

void GLGui::destroyWindow(std::size_t id) {
    glfwDestroyWindow(glfwWindows[id]);
}

void GLGui::makeWindowContextCurrent(std::size_t id) {
    glfwMakeContextCurrent(glfwWindows[id]);
}

void GLGui::swapBuffers(std::size_t id) {

    glfwSwapBuffers(glfwWindows[id]);
}

void GLGui::waitEvents() {

    glfwWaitEvents();
}

void GLGui::waitEventsTimeout(double timeout) {
    glfwWaitEventsTimeout(timeout);
}

void GLGui::keyCallback(GLFWwindow* window, int key, int scancode, int action,
                        int mods) {
    GLGui::currentEventWindow = window;
    GLGui::keyStore.key = key;
    GLGui::keyStore.keyActionStore = action;
    std::size_t currentWindowID =
        reinterpret_cast<int64_t>(glfwGetWindowUserPointer(window));
    // we know the window so can call this directly as opposed to
    // processEvents which is called from app (we could change that
    // from a pull method to a push).
    glfwMakeContextCurrent(window);
    cout << "boo5\n";

    Application::handleEvent(currentWindowID, GLGui::keyStore);
}

void GLGui::MouseButtonCallback(
    GLFWwindow* window, int button, int action,
    int mods) { // here set doSelection to true and do the call in the app loop.
                // not meant to do opengl stuff in callbacks.

    GLGui::currentEventWindow = window;
    GLGui::keyStore.buttonStore = button;
    GLGui::keyStore.mouseActionStore = action;
    std::size_t currentWindowID =
        reinterpret_cast<int64_t>(glfwGetWindowUserPointer(window));
    // we know the window so can call this directly as opposed to
    // processEvents which is called from app (we could change that
    // from a pull method to a push).
    glfwMakeContextCurrent(window);

    auto NOW = std::chrono::steady_clock::now();
    chrono::duration<double, std::milli> fp_ms = NOW - last;
    auto counted =
        chrono::duration_cast<std::chrono::milliseconds>(fp_ms).count();
    if (counted < 16) {
        ready = false;
        return;
    } else {

        ready = true;
    }

    Application::handleEvent(currentWindowID, GLGui::keyStore);
    last = std::chrono::steady_clock::now();
}

void GLGui::MousePosCallback(GLFWwindow* window, double xpos, double ypos) {
    GLGui::currentEventWindow = window;
    GLGui::keyStore.Mx = xpos;
    GLGui::keyStore.My = ypos;
    std::size_t currentWindowID = reinterpret_cast<int64_t>(
        glfwGetWindowUserPointer(GLGui::currentEventWindow));

    // we have to set the current window context, because when an event occurs,
    // it might not be in the right window loop (they can occur anytime
    glfwMakeContextCurrent(window);
    auto NOW = std::chrono::steady_clock::now();
    chrono::duration<double, std::milli> fp_ms = NOW - last;
    auto counted =
        chrono::duration_cast<std::chrono::milliseconds>(fp_ms).count();
    if (counted < 16) {
        ready = false;
        return;
    } else {

        ready = true;
    }
    Application::handleEvent(currentWindowID, GLGui::keyStore);
    last = std::chrono::steady_clock::now();
}

void GLGui::processEvents() { // called in main loop and forwards events to
                              // windows for further handling
    std::size_t currentWindowID = reinterpret_cast<int64_t>(
        glfwGetWindowUserPointer(GLGui::currentEventWindow));

    // we have to set the current window context, because when an event occurs,
    // it might not be in the right window loop (they can occur anytime
    makeWindowContextCurrent(currentWindowID);
    Application::handleEvent(currentWindowID, GLGui::keyStore);
}

void GLGui::framebuffer_size_callback(GLFWwindow* window, int width,
                                      int height) {
    GLGui::windowRect.size.width = width;
    GLGui::windowRect.size.height = height;
    std::size_t currentWindowID =
        reinterpret_cast<int64_t>(glfwGetWindowUserPointer(window));
    // we know the window so can call this directly as opposed to
    // processEvents which is called from app (we could change that
    // from a pull method to a push).
    glfwMakeContextCurrent(window);
    Application::framebuffer_size_callback(currentWindowID, width, height);
    windowResized = true;
}

// to get around calling non static functions glfw lets us store pointers
void GLGui::window_refresh_callback(GLFWwindow* window) {
    std::size_t currentWindowID =
        reinterpret_cast<int64_t>(glfwGetWindowUserPointer(window));
    auto NOW = std::chrono::steady_clock::now();
    chrono::duration<double, std::milli> fp_ms = NOW - last;
    auto counted =
        chrono::duration_cast<std::chrono::milliseconds>(fp_ms).count();
    if (counted < 16) {
        ready = false;
        return;
    } else {
        ready = true;
    }
    Application::forceRefresh(currentWindowID);
    last = std::chrono::steady_clock::now();
}

void GLGui::refresh() {

    eventWindow->ForceRefresh();
}

void GLGui::resizeWindow(int width, int height) {
}

void GLGui::addWindow(UIWindow* window) {

    // here maybe add UIwindow id checking so we can forwad messages to correct
    // window
    GLGui::eventWindow = window;
    glfwSetWindowUserPointer(window->getWindow(), window);
}

void GLGui::CloseGUI(int return_code) {
    glfwTerminate();
    exit(return_code);
}

void GLGui::setCallBacks(GLFWwindow* window) {

    glfwSetKeyCallback(window, GLGui::keyCallback);
    glfwSetMouseButtonCallback(window, GLGui::MouseButtonCallback);
    glfwSetCursorPosCallback(window, GLGui::MousePosCallback);
    glfwSetWindowSizeCallback(window, GLGui::framebuffer_size_callback);
    glfwSetWindowRefreshCallback(window, GLGui::window_refresh_callback);
}

void GLGui::enableMousePosCallback(GLFWwindow* window) {
    glfwSetCursorPosCallback(window, GLGui::MousePosCallback);
}

void GLGui::disableMousePosCallback(GLFWwindow* window) {
    glfwSetCursorPosCallback(window, NULL);
}

void GLGui::enableMouseButtonCallback(GLFWwindow* window) {
    glfwSetMouseButtonCallback(window, GLGui::MouseButtonCallback);
}

void GLGui::disableMouseButtonCallback(GLFWwindow* window) {
    glfwSetMouseButtonCallback(window, NULL);
}
