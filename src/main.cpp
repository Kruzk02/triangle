#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <shader.h>
#include <window.h>
#include <mesh.h>

void processInput(GLFWwindow* window);

int main() {
    const Window window{900, 720};

    const Shader myShader("asset/shader/shader.vs", "asset/shader/shader.fs");

    const std::vector vertices = {
        -0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 0.0f,
        0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 0.0f,
        0.0f,  0.5f, 0.0f, 0.0f, 0.0f, 1.0f
    };

     const std::vector<unsigned int> indices = {
        0, 1, 2
    };

    const Mesh mesh(vertices, indices);

    GLint timeLoc = glGetUniformLocation(myShader.ID, "uTime");

    while(!window.shouldClose()) {
        processInput(window.getNativeWindow());

        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        float time = glfwGetTime();
        glUniform1f(timeLoc, time);
        myShader.use();
        mesh.draw();

        glfwPollEvents();
        glfwSwapBuffers(window.getNativeWindow());
    }

    return 0;
}

void processInput(GLFWwindow *window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
}
