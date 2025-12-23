#pragma once
#include "glad/glad.h"
#include "glm/vec3.hpp"
#include <glm/gtc/matrix_transform.hpp>

enum class CameraMovement {
    FORWARD,
    BACKWARD,
    LEFT,
    RIGHT
};

struct CameraSettings {
    float yaw = -90.0f;
    float pitch = 0.0f;
    float speed = 2.5f;
    float sensitivity = 0.1f;
    float zoom = 45.0f;
};

class Camera {
public:
    explicit Camera(glm::vec3 position,
                    glm::vec3 up,
                    const CameraSettings &settings = {}) : Camera(position.x, position.y, position.z, up.x, up.y, up.z,
                                                                  settings.yaw, settings.pitch, settings) {
        updateCameraVectors();
    }

    Camera(float posX, float posY, float posZ,
        float upX, float upY, float upZ,
        float yaw, float pitch,
        const CameraSettings& settings = {}) :
    front(glm::vec3(0.0f, 0.0f, -1.0f)), movementSpeed(settings.speed), zoom(settings.zoom), mouseSensitivity(settings.sensitivity) {
        this->position = glm::vec3(posX, posY, posZ);
        this->worldUp = glm::vec3(upX, upY, upZ);
        this->yaw = yaw;
        this->pitch = pitch;
        updateCameraVectors();
    }

    [[nodiscard]] glm::mat4 getViewMatrix() const {
        return glm::lookAt(position, position + front, up);
    }

    void processKeyboard(const CameraMovement direction, const float deltaTime) {
        const float velocity = movementSpeed * deltaTime;
        if (direction == CameraMovement::FORWARD) position += front * velocity;
        if (direction == CameraMovement::BACKWARD) position -= front * velocity;
        if (direction == CameraMovement::LEFT) position -= right * velocity;
        if (direction == CameraMovement::RIGHT) position += right * velocity;
    }

    void processMouseMovement(float xOffset, float yOffset, const GLboolean constrainPitch = true) {
        xOffset *= mouseSensitivity;
        yOffset *= mouseSensitivity;

        yaw += xOffset;
        pitch += yOffset;

        if (constrainPitch) {
            if (pitch > 89.0f) pitch = 89.0f;
            if (pitch < -89.0f) pitch = -89.0f;
        }

        updateCameraVectors();
    }

    void processMouseScroll(const float yOffset) {
        zoom -= yOffset;
        if (zoom < 1.0f) zoom = 1.0f;
        if (zoom > 45.0f) zoom = 45.0f;
    }

    [[nodiscard]] const glm::vec3& getPosition() const { return position; }
    [[nodiscard]] const glm::vec3& getFront() const { return front; }

    [[nodiscard]] float getZoom() const { return zoom; }
private:
    glm::vec3 position{};
    glm::vec3 front;
    glm::vec3 up{};
    glm::vec3 right{};
    glm::vec3 worldUp{};

    float yaw;
    float pitch;
    float movementSpeed;
    float mouseSensitivity{};
    float zoom;

    void updateCameraVectors() {
        glm::vec3 v_front;
        v_front.x = glm::cos(glm::radians(yaw)) * glm::cos(glm::radians(pitch));
        v_front.y = glm::sin(glm::radians(pitch));
        v_front.z = glm::sin(glm::radians(yaw)) * glm::cos(glm::radians(pitch));
        front = glm::normalize(v_front);

        right = glm::normalize(glm::cross(front, worldUp));
        up    = glm::normalize(glm::cross(right, front));
    }
};