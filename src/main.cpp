#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <shader.h>
#include <window.h>
#include <mesh.h>
#include <texture.h>
#include <transform.h>

float getDeltaTime();
void processInput(GLFWwindow* window);

int main() {
    const Window window{900, 720};

    glEnable(GL_DEPTH_TEST);

    const Shader myShader("asset/shader/shader.vs", "asset/shader/shader.fs");

    const std::vector vertices = {
        -0.5f,-0.5f,-0.5f,  0.0f,0.0f,
         0.5f,-0.5f,-0.5f,  1.0f,0.0f,
         0.5f, 0.5f,-0.5f,  1.0f,1.0f,
        -0.5f, 0.5f,-0.5f,  0.0f,1.0f,

        -0.5f,-0.5f, 0.5f,  0.0f,0.0f,
         0.5f,-0.5f, 0.5f,  1.0f,0.0f,
         0.5f, 0.5f, 0.5f,  1.0f,1.0f,
        -0.5f, 0.5f, 0.5f,  0.0f,1.0f,

        -0.5f, 0.5f, 0.5f,  1.0f,0.0f,
        -0.5f, 0.5f,-0.5f,  1.0f,1.0f,
        -0.5f,-0.5f,-0.5f,  0.0f,1.0f,
        -0.5f,-0.5f, 0.5f,  0.0f,0.0f,

         0.5f, 0.5f, 0.5f,  1.0f,0.0f,
         0.5f, 0.5f,-0.5f,  1.0f,1.0f,
         0.5f,-0.5f,-0.5f,  0.0f,1.0f,
         0.5f,-0.5f, 0.5f,  0.0f,0.0f,

        -0.5f,-0.5f,-0.5f,  0.0f,1.0f,
         0.5f,-0.5f,-0.5f,  1.0f,1.0f,
         0.5f,-0.5f, 0.5f,  1.0f,0.0f,
        -0.5f,-0.5f, 0.5f,  0.0f,0.0f,

        -0.5f, 0.5f,-0.5f,  0.0f,1.0f,
         0.5f, 0.5f,-0.5f,  1.0f,1.0f,
         0.5f, 0.5f, 0.5f,  1.0f,0.0f,
        -0.5f, 0.5f, 0.5f,  0.0f,0.0f
    };

    const std::vector<unsigned int> indices = {
        0,  1,  2,  2,  3,  0,
        4,  5,  6,  6,  7,  4,
        8,  9, 10, 10, 11,  8,
       12, 13, 14, 14, 15, 12,
       16, 17, 18, 18, 19, 16,
       20, 21, 22, 22, 23, 20
   };

    const VertexLayout layout {
        {
            {0, 3, GL_FLOAT, GL_FALSE, 0},
            {1, 2, GL_FLOAT, GL_FALSE, 3 * sizeof(float)}
        },
        5 * sizeof(float)
    };

    const Mesh mesh(vertices, indices, layout);
    const Texture texture {"asset/wall.jpg"};

    myShader.setInt("uTexture", 0);

    Transform transform;

    const glm::mat4 view = glm::lookAt(
        glm::vec3(0, 0, 3),
        glm::vec3(0, 0, 0),
        glm::vec3(0, 1, 0)
    );

    const glm::mat4 projection = glm::perspective(
        glm::radians(60.0f),
        900 / 720.0f,
        0.1f,
        100.0f
    );

    while(!window.shouldClose()) {
        processInput(window.getNativeWindow());

        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        const float deltaTime = getDeltaTime();
        transform.rotation.y += glm::radians(45.0f) * deltaTime;

        myShader.use();
        myShader.setMat4("uModel", transform.matrix());
        myShader.setMat4("uView", view);
        myShader.setMat4("uProjection", projection);
        texture.bind();
        mesh.draw();

        glfwPollEvents();
        glfwSwapBuffers(window.getNativeWindow());
    }

    return 0;
}

float getDeltaTime() {
    static float last = 0.0f;
    const auto now = static_cast<float>(glfwGetTime());
    const float dt = now - last;
    last = now;
    return dt;
}

void processInput(GLFWwindow *window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
}
