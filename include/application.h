#pragma once

struct AppConfig {
    int width = 900;
    int height = 720;
    const char* shaderVertex = "asset/shader/shader.vs";
    const char* shaderFragment = "asset/shader/shader.fs";
};

struct MouseState {
    float lastX = 400.0f;
    float lastY = 300.0f;
    bool firstMouse = true;
};

class Application {
public:
    static void run();
};