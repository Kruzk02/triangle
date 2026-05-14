#pragma once

#include "glad/glad.h"
#include <GLFW/glfw3.h>

struct WindowData
{
    int width;
    int height;
};

class Window
{
  public:
    explicit Window(int width, int height, const char *title = "window");
    ~Window();

    [[nodiscard]] auto shouldClose() const -> bool
    {
        return glfwWindowShouldClose(window);
    }

    void swapBuffers() const
    {
        glfwSwapBuffers(window);
    }

    [[nodiscard]] auto getNativeWindow() const -> GLFWwindow *
    {
        return window;
    }

    [[nodiscard]] auto getWindowData() const -> WindowData
    {
        return windowData;
    }

  private:
    GLFWwindow *window;
    WindowData windowData;

    static void framebuffer_size_callback(GLFWwindow *window, int width, int height);
};
