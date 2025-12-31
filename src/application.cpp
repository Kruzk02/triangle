#include <application.h>

#include "camera.h"
#include "layout.h"
#include "mesh.h"
#include "shader.h"
#include "texture.h"
#include "transform.h"
#include "window.h"
#include "GLFW/glfw3.h"

float deltaTime = 0.0f;
float lastFrame = 0.0f;

constexpr float FOV = 45.0f;
constexpr float NEAR_PLANE = 0.1f;
constexpr float FAR_PLANE = 100.0f;

MouseState mouseState;

void updateDeltaTime();
void processInput(GLFWwindow* window, Camera& camera);
void mouseCallback(GLFWwindow* window, double xPos, double yPos);
void scrollCallBack(GLFWwindow* window, double xOffset, double yOffset);

void Application::run() {
    constexpr AppConfig appConfig;

    const Window window {appConfig.width, appConfig.height};
    const auto nativeWindow = window.getNativeWindow();
    glEnable(GL_DEPTH_TEST);

    const Shader myShader(appConfig.shaderVertex, appConfig.shaderFragment);

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
        updateDeltaTime();

        processInput(nativeWindow, camera);

        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        transform.rotation.y += glm::radians(FOV) * deltaTime;

        myShader.use();

        myShader.setMat4("uModel", transform.matrix());
        myShader.setMat4("uView", camera.getViewMatrix());
        myShader.setMat4("uProjection", glm::perspective(
            glm::radians(camera.getZoom()),
            900.0f / 720.0f,
            NEAR_PLANE,
            FAR_PLANE
        ));

        texture.bind();
        mesh.draw();

        glfwPollEvents();
        glfwSwapBuffers(nativeWindow);
    }
}

void updateDeltaTime() {
    const auto now = static_cast<float>(glfwGetTime());
    deltaTime = now - lastFrame;
    lastFrame = now;
}

void processInput(GLFWwindow *window, Camera &camera)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
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

    if (mouseState.firstMouse) {
        mouseState.lastX = static_cast<float>(xPos);
        mouseState.lastY = static_cast<float>(yPos);
        mouseState. firstMouse = false;
        return;
    }

    const float xOffset = static_cast<float>(xPos) - mouseState.lastX;
    const float yOffset = mouseState.lastY - static_cast<float>(yPos);

    mouseState.lastX = static_cast<float>(xPos);
    mouseState.lastY = static_cast<float>(yPos);

    camera->processMouseMovement(xOffset, yOffset);
}

void scrollCallBack(GLFWwindow* window, double xOffset, const double yOffset) {
    auto* camera = static_cast<Camera*>(glfwGetWindowUserPointer(window));
    camera -> processMouseScroll(static_cast<float>(yOffset));
}
