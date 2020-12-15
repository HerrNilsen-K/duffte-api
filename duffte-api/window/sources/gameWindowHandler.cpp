#define GLEW_STATIC       
#include <GL/glew.h>
#include "gameWindowHandler.hpp"
#include "../../util.hpp"
#include <iostream>
#include "../../primitives/primitives.hpp"

extern duffte::trinagleCreation *triangleTemplate;

namespace duffte
{
    void gameWindow::init(int p_width, int p_height, std::string p_name, int p_flags)
    {
        m_window.setDimensions(p_width, p_height);
        m_window.setTitle(p_name);
        m_window.createWindow();
        m_window.makeContextCurrent();

        m_hold = false;
        m_flags = 0;

        glewInit();
        //Set user pointer to access "this" in "staticResizeCall"
        glfwSetWindowUserPointer(m_window.ID(), reinterpret_cast<void *>(this));

        glfwSetWindowSizeCallback(m_window.ID(), [](GLFWwindow *window, int width, int height) {
            gameWindow *gw = reinterpret_cast<gameWindow *>(glfwGetWindowUserPointer(window));
            if (gw)
                gw->resizeCall(width, height);
            else
            {
                ERROR_LOG("Error: gw in duffte::gameWindow::init() -> glfwSetWindowSizeCallback() == NULL");
            }
        });

        glfwSetKeyCallback(m_window.ID(), [](GLFWwindow *window, int key, int scancode, int action, int mods) {
            gameWindow *gw = reinterpret_cast<gameWindow *>(glfwGetWindowUserPointer(window));
            if (gw)
                gw->keyCall(key, scancode, action, mods);
            else
                ERROR_LOG("Error: gw in duffte::gameWindow::init() glfwSetKeyCallback() == NULL");
        });
        if (p_flags & flags::GRAPHICS)
            m_flags |= GL_COLOR_BUFFER_BIT;
        if (p_flags & flags::DRAW_3D)
            m_flags |= GL_DEPTH_BUFFER_BIT;

        //Init global primitives
        triangleTemplate = new duffte::trinagleCreation;
    }

    void gameWindow::startEngine()
    {
        onStart();
        startRenderLoop();
        onExit();

        terminate();
    }

    duffte::key gameWindow::getCurrentKey()
    {
        return m_currentKey;
    }

    int gameWindow::getCurrentKeyMode()
    {
        return m_currentKeyMode;
    }

    bool gameWindow::keyIsHeld()
    {
        return m_hold;
    }

    bool gameWindow::startRenderLoop()
    {
        while (m_window.runs())
        {
            render();
        }
        return true;
    }

    void gameWindow::resizeCall(int width, int height)
    {
        render();
        glViewport(0, 0, width, height);
    }

    void gameWindow::render()
    {
        glClear(m_flags);
        mainLoop();
        m_window.swapBuffers();
        m_window.pollEvents();
    }
    
    void gameWindow::terminate() 
    {
        delete triangleTemplate;
    }

    void gameWindow::keyCall(int key, int scancode, int action, int mods)
    {
        m_currentKey = (duffte::key)key;
        m_currentKeyMode = action;
        if (action == GLFW_PRESS)
        {
            m_hold = true;
        }
        else if (action == GLFW_RELEASE)
        {
            m_hold = false;
        }
    }

} // namespace duffte