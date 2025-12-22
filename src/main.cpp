#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <shader.h>
#include <window.h>
#include <mesh.h>
#include <texture.h>
#include <transform.h>

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

    const VertexLayout layout {
        {
            {0, 3, GL_FLOAT, GL_FALSE, 0},
            {1, 2, GL_FLOAT, GL_FALSE, 3 * sizeof(float)}
        },
        6 * sizeof(float)
    };

    const Mesh mesh(vertices, indices, layout);
    const Texture texture {"asset/wall.jpg"};

    myShader.setInt("uTexture", 0);

    const GLint timeLoc = glGetUniformLocation(myShader.ID, "uTime");

    Transform transform;


    while(!window.shouldClose()) {
        processInput(window.getNativeWindow());

        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        static float lastTime = 0.0f;
        const auto currentTime = static_cast<float>(glfwGetTime());
        const auto deltaTime = currentTime - lastTime;
        lastTime = currentTime;

        glUniform1f(timeLoc, currentTime);
        transform.rotation.y += glm::radians(45.0f) * deltaTime;

        myShader.use();
        myShader.setMat4("transform", transform.matrix());
        texture.bind();
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
