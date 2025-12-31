#include <application.h>

#include "camera.h"
#include "layout.h"
#include "mesh.h"
#include "shader.h"
#include "texture.h"
#include "transform.h"
#include "window.h"
#include "GLFW/glfw3.h"

float lastX = 400.0f;
float lastY = 300.0f;
bool firstMouse = true;

float getDeltaTime();
void processInput(GLFWwindow* window);
void processCameraInput(GLFWwindow* window, Camera& camera, float deltaTime);
void mouseCallback(GLFWwindow* window, double xPos, double yPos);
void scrollCallBack(GLFWwindow* window, double xOffset, double yOffset);

void Application::run() {
    const Window window {900, 720};
    const auto nativeWindow = window.getNativeWindow();
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

    const Mesh mesh {vertices, indices, layout};
    const Texture texture {"asset/wall.jpg"};

    myShader.setInt("uTexture", 0);

    Transform transform;
    Camera camera {glm::vec3(0, 0, 3), glm::vec3(0, 1, 0)};

    glfwSetWindowUserPointer(nativeWindow, &camera);
    glfwSetCursorPosCallback(nativeWindow, mouseCallback);
    glfwSetScrollCallback(nativeWindow, scrollCallBack);
    glfwSetInputMode(nativeWindow, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    while (!window.shouldClose()) {
        const float deltaTime = getDeltaTime();

        processInput(nativeWindow);
        processCameraInput(nativeWindow, camera, deltaTime);

        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        transform.rotation.y += glm::radians(45.0f) * deltaTime;

        myShader.use();

        myShader.setMat4("uModel", transform.matrix());
        myShader.setMat4("uView", camera.getViewMatrix());
        myShader.setMat4("uProjection", glm::perspective(
            glm::radians(camera.getZoom()),
            900.0f / 720.0f,
            0.1f,
            100.0f
        ));

        texture.bind();
        mesh.draw();

        glfwPollEvents();
        glfwSwapBuffers(nativeWindow);
    }
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

void processCameraInput(GLFWwindow *window, Camera &camera, const float deltaTime) {
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        camera.processKeyboard(CameraMovement::FORWARD, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        camera.processKeyboard(CameraMovement::BACKWARD, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        camera.processKeyboard(CameraMovement::LEFT, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        camera.processKeyboard(CameraMovement::RIGHT, deltaTime);
}

void mouseCallback(GLFWwindow* window, const double xPos, const double yPos) {
    auto* camera =
        static_cast<Camera*>(glfwGetWindowUserPointer(window));

    if (!camera) return;

    if (firstMouse) {
        lastX = static_cast<float>(xPos);
        lastY = static_cast<float>(yPos);
        firstMouse = false;
        return;
    }

    const float xOffset = static_cast<float>(xPos) - lastX;
    const float yOffset = lastY - static_cast<float>(yPos);

    lastX = static_cast<float>(xPos);
    lastY = static_cast<float>(yPos);

    camera->processMouseMovement(xOffset, yOffset);
}

void scrollCallBack(GLFWwindow* window, double xOffset, const double yOffset) {
    auto* camera = static_cast<Camera*>(glfwGetWindowUserPointer(window));
    camera -> processMouseScroll(static_cast<float>(yOffset));
}
