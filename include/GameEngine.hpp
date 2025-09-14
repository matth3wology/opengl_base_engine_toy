#pragma once

#include "Hero.hpp"
#include "ShaderPipeline.hpp"
#include <GLFW/glfw3.h>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>

class GEngine {
  GLFWwindow *window;
  std::unique_ptr<ShaderPipeline> shader;
  std::unique_ptr<Hero> hero;

public:
  GEngine() {
    // Initialize GLFW
    if (!glfwInit()) {
      std::cerr << "Failed to init GLFW\n";
      glfwTerminate();
      exit(-1);
    }

    // Request at least OpenGL 3.3 core profile
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    // Create a new Window
    window = glfwCreateWindow(1243, 700, "Game Engine", nullptr, nullptr);
    if (!window) {
      std::cerr << "Failed to create window\n";
      glfwTerminate();
      exit(-1);
    }

    glfwMakeContextCurrent(window);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
      std::cerr << "Failed to initialize GLAD\n";
      exit(-1);
    }

    const char *vertexShaderSource = R"(
      #version 330 core
      layout(location = 0) in vec3 aPos;

      uniform mat4 transformation;
      void main() {
      gl_Position = transformation * vec4(aPos, 1.0);
        }
    )";

    const char *fragmentShaderSource = R"(
        #version 330 core

        out vec4 FragColor;

        void main() {
            FragColor = vec4(1.0,0.3,0.2,1.0);
        }
    )";

    shader = std::make_unique<ShaderPipeline>(vertexShaderSource,
                                              fragmentShaderSource);

    hero = std::make_unique<Hero>(MeshFactory::createPyramid(),
                                  std::make_unique<XboxController>());
  }

  ~GEngine() {
    glfwDestroyWindow(window);
    glfwTerminate();
  }

  GEngine(const GEngine &) = delete;
  GEngine &operator=(const GEngine &) = delete;

  GEngine(GEngine &&) = default;            // allow moves
  GEngine &operator=(GEngine &&) = default; // allow moves

  GLFWwindow *get_window() { return window; }

  void render() {
    glClearColor(0.1f, 0.2f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    shader->use();

    hero->render(*shader);
  }

  void update(float deltaTime) { hero->update(window, deltaTime); }
};
