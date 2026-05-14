#include "window.hpp"
#include "GLFW/glfw3.h"

#include <stdexcept>

Window::Window(const int width, const int height, const char *title)
{
    if (!glfwInit())
    {
        throw std::runtime_error("Failed to initialize GLFW");
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWmonitor *monitor = nullptr;

    int finalWidth = width;
    int finalHeight = height;

    if (width == 0 && height == 0)
    {
        monitor = glfwGetPrimaryMonitor();
        const GLFWvidmode *mode = glfwGetVideoMode(monitor);

        finalWidth = mode->width;
        finalHeight = mode->height;
    }

    windowData.width = finalWidth;
    windowData.height = finalHeight;

    window = glfwCreateWindow(finalWidth, finalHeight, title, monitor, nullptr);
    if (window == nullptr)
    {
        throw std::runtime_error("Failed to create GLFW window");
    }

    if (GLFWmonitor *monitor = glfwGetPrimaryMonitor())
    {
        if (const GLFWvidmode *mode = glfwGetVideoMode(monitor))
        {
            const int xPos = (mode->width - width) / 2;
            const int yPos = (mode->height - height) / 2;
            glfwSetWindowPos(window, xPos, yPos);
        }
    }

    glfwMakeContextCurrent(window);
    glfwSetWindowUserPointer(window, &windowData);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    if (!gladLoadGLLoader(reinterpret_cast<GLADloadproc>(glfwGetProcAddress)))
    {
        throw std::runtime_error("Failed to initialize GLAD");
    }
}

Window::~Window()
{
    if (window)
        glfwDestroyWindow(window);
    glfwTerminate();
}

void Window::framebuffer_size_callback(GLFWwindow *window, const int width, const int height)
{
    if (width == 0 || height == 0)
        return;

    auto *data = static_cast<WindowData *>(glfwGetWindowUserPointer(window));
    if (!data)
    {
        printf("NO USER POINTER\n");
        return;
    }
    data->width = width;
    data->height = height;
    glViewport(0, 0, width, height);
}
