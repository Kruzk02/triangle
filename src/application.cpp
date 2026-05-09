#include "application.hpp"

#include "GLFW/glfw3.h"
#include "camera.hpp"
#include "input.hpp"
#include "layout.hpp"
#include "mesh.hpp"
#include "model.hpp"
#include "shader.hpp"
#include "transform.hpp"
#include <utility>

namespace RenderDefaults
{
constexpr glm::vec4 ClearColor{0.2f, 0.3f, 0.3f, 1.0f};

constexpr glm::vec3 LightColor{2.0f, 2.0f, 2.0f};
constexpr glm::vec3 LightPosition{2.0f, 3.0f, 2.0f};
} // namespace RenderDefaults

namespace CameraDefaults
{
constexpr glm::vec3 Position{0.0f, 0.5f, 3.0f};
constexpr glm::vec3 Up{0.0f, 1.0f, 0.0f};
} // namespace CameraDefaults

namespace AssetPaths
{
constexpr const char *GridVertex = "asset/shader/grid.vs";
constexpr const char *GridFragment = "asset/shader/grid.fs";
constexpr const char *BackpackModel = "asset/models/backpack/Backpack.obj";
} // namespace AssetPaths

namespace SceneDefaults
{
constexpr float ModelHeightOffset = 1.5f;
}

namespace TransformLimits
{
constexpr float PitchMin = glm::radians(-89.0f);
constexpr float PitchMax = glm::radians(89.0f);
} // namespace TransformLimits

namespace ProjectionDefaults
{
constexpr float NearPlane = 0.1f;
constexpr float FarPlane = 100.0f;
} // namespace ProjectionDefaults

constexpr float ROT_SPEED = glm::radians(90.0f);

void key_callback(GLFWwindow *window, int key, int scancode, int action, int mods) // NOLINT
{
    Input::setKey({.key = key, .action = action});
}

Application::Application(const AppConfig &config)
    : lightingShader({.vertexPath = config.shaderVertex, .fragmentPath = config.shaderFragment}),
      gridShader({.vertexPath = AssetPaths::GridVertex, .fragmentPath = AssetPaths::GridFragment}),
      gridMesh(createGridMesh()), config(config), window(config.width, config.height),
      camera(CameraPosition{CameraDefaults::Position.x, CameraDefaults::Position.y,
                            CameraDefaults::Position.z},
             CameraUp{CameraDefaults::Up.x, CameraDefaults::Up.y, CameraDefaults::Up.z})
{
    auto *nativeWindow = window.getNativeWindow();

    glfwSetWindowUserPointer(nativeWindow, this);
    glfwSetCursorPosCallback(nativeWindow, mouseCallback);
    glfwSetScrollCallback(nativeWindow, scrollCallback);
    glfwSetKeyCallback(nativeWindow, key_callback);
    glfwSetInputMode(nativeWindow, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    glEnable(GL_DEPTH_TEST);

    glDepthFunc(GL_LESS);
    glEnable(GL_FRAMEBUFFER_SRGB);
}

void Application::setup()
{
    Model model;
    model.loadFromFile(AssetPaths::BackpackModel);
    model.transform.position.y = SceneDefaults::ModelHeightOffset;

    scene.addModel(std::move(model));

    Input::init();
}

void Application::update()
{
    Input::update();
    updateDeltaTime();
    processInput();
}

void Application::render()
{
    glClearColor(RenderDefaults::ClearColor.r, RenderDefaults::ClearColor.g,
                 RenderDefaults::ClearColor.b, RenderDefaults::ClearColor.a);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glm::mat4 view = camera.getViewMatrix();
    glm::mat4 projection =
        glm::perspective(glm::radians(camera.getZoom()),
                         static_cast<float>(config.width) / static_cast<float>(config.height),
                         ProjectionDefaults::NearPlane, ProjectionDefaults::FarPlane);

    gridShader.use();
    gridShader.setMat4("MVP", projection * view * glm::mat4(1.0f));
    gridMesh.draw();

    lightingShader.use();
    lightingShader.setVec3("lightColor", RenderDefaults::LightColor);
    lightingShader.setVec3("lightPos", RenderDefaults::LightPosition);
    lightingShader.setVec3("viewPos", camera.getPosition());
    lightingShader.setMat4("projection", projection);
    lightingShader.setMat4("view", view);

    for (auto &model : scene.getModels())
    {
        model.transform.rotation.x = glm::clamp(
            model.transform.rotation.x, TransformLimits::PitchMin, TransformLimits::PitchMax);
        lightingShader.setMat4("model", model.transform.matrix());
        model.draw(lightingShader);
    }
}

void Application::run()
{
    setup();

    while (!window.shouldClose())
    {
        glfwPollEvents();
        update();
        render();
        glfwSwapBuffers(window.getNativeWindow());
    }
}

auto Application::createGridMesh() -> Mesh
{

    const std::vector<float> gridVertices = {-1000, 0, -1000, 1000,  0, -1000,
                                             1000,  0, 1000,  -1000, 0, 1000};

    std::vector<Vertex> gridOut;
    for (size_t i = 0; i < gridVertices.size(); i += 3)
    {
        Vertex v{};

        v.Position = glm::vec3(gridVertices[i + 0], gridVertices[i + 1], gridVertices[i + 2]);

        v.Normal = glm::vec3(0.0f, 1.0f, 0.0f);
        v.TexCoord = glm::vec2(0.0f);
        v.Tangent = glm::vec3(0.0f);
        v.BiTangent = glm::vec3(0.0f);

        for (int j = 0; j < MAX_BONE_INFLUENCE; j++)
        {
            v.mBoneIds[j] = -1;
            v.mWeights[j] = 0.0f;
        }

        gridOut.push_back(v);
    }

    const std::vector<unsigned int> gridIndices = {0, 1, 2, 2, 3, 0};

    const VertexLayout gridLayout({{0, 3, GL_FLOAT, GL_FALSE, offsetof(Vertex, Position)}},
                                  sizeof(Vertex));

    return {gridOut, gridIndices, gridLayout};
}

void Application::updateDeltaTime()
{
    const auto now = static_cast<float>(glfwGetTime());
    deltaTime = now - lastFrame;
    lastFrame = now;
}

void Application::processInput()
{
    auto *w = window.getNativeWindow();

    if (Input::isActionPressed("Quit"))
        glfwSetWindowShouldClose(w, true);

    if (Input::isActionHeld("MoveForward"))
        camera.processKeyboard(CameraMovement::FORWARD, deltaTime);

    if (Input::isActionHeld("MoveBackward"))
        camera.processKeyboard(CameraMovement::BACKWARD, deltaTime);

    if (Input::isActionHeld("MoveLeft"))
        camera.processKeyboard(CameraMovement::LEFT, deltaTime);

    if (Input::isActionHeld("MoveRight"))
        camera.processKeyboard(CameraMovement::RIGHT, deltaTime);
}

void Application::mouseCallback(GLFWwindow *window, const double xPos, const double yPos)
{
    auto *app = static_cast<Application *>(glfwGetWindowUserPointer(window));
    if (!app)
        return;

    auto &[lastX, lastY, firstMouse] = app->mouseState;

    if (firstMouse)
    {
        lastX = static_cast<float>(xPos);
        lastY = static_cast<float>(yPos);
        firstMouse = false;
        return;
    }

    const float xOffset = static_cast<float>(xPos) - lastX;
    const float yOffset = lastY - static_cast<float>(yPos);

    lastX = static_cast<float>(xPos);
    lastY = static_cast<float>(yPos);

    app->camera.processMouseMovement(xOffset, yOffset);
}

void Application::scrollCallback(GLFWwindow *window, double xOffset, double yOffset)
{
    auto *app = static_cast<Application *>(glfwGetWindowUserPointer(window));
    if (!app)
        return;

    app->camera.processMouseScroll(static_cast<float>(yOffset));
}
