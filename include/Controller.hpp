#pragma once

#include "Character.hpp"

#define SDL_MAIN_HANDLED

#include <GLFW/glfw3.h>
#include <SDL2/SDL.h>
#include <iostream>

class Controller {
public:
  virtual void update(Character &Character, GLFWwindow *window,
                      float deltaTime) = 0;
};

class KeyboardController : public Controller {
public:
  void update(Character &character, GLFWwindow *window,
              float deltaTime) override {
    float dx = 0.0f, dy = 0.0f;

    // WASD movement
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
      dy += 1.0f;
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
      dy -= 1.0f;
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
      dx -= 1.0f;
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
      dx += 1.0f;

    // Rotation
    if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS)
      character.rotation += glm::radians(90.0f) * deltaTime;
    if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS)
      character.rotation -= glm::radians(90.0f) * deltaTime;

    character.rotation = fmod(character.rotation, glm::two_pi<float>());

    // Move relative to rotation
    glm::vec3 right =
        glm::vec3(cos(character.rotation), sin(character.rotation), 0.0f);
    glm::vec3 forward =
        glm::vec3(-sin(character.rotation), cos(character.rotation), 0.0f);

    glm::vec3 velocity = (dy * forward + dx * right) * deltaTime * 2.0f;
    character.position += velocity;
  }
};

class XboxController : public Controller {
  SDL_GameController *controller = nullptr;

public:
  XboxController() {
    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_GAMECONTROLLER) < 0) {
      std::cerr << "Failed to initialize SDL: " << SDL_GetError() << std::endl;
      exit(-1);
    }

    // Open first available controller
    for (int i = 0; i < SDL_NumJoysticks(); ++i) {
      if (SDL_IsGameController(i)) {
        controller = SDL_GameControllerOpen(i);
        if (controller) {
          std::cout << "Opened controller: "
                    << SDL_GameControllerName(controller) << std::endl;
          break;
        }
      }
    }

    if (!controller) {
      std::cerr << "No game controller found!" << std::endl;
      SDL_Quit();
      exit(-1);
    }
  }

  ~XboxController() {
    if (controller) {
      SDL_GameControllerClose(controller);
    }
    SDL_Quit();
  }

  void update(Character &character, GLFWwindow * /*window*/,
              float deltaTime) override {
    // Poll events for button presses/releases
    SDL_Event e;
    while (SDL_PollEvent(&e)) {
      if (e.type == SDL_CONTROLLERBUTTONDOWN) {
        std::cout << "Button pressed: " << (int)e.cbutton.button << std::endl;
      } else if (e.type == SDL_CONTROLLERBUTTONUP) {
        std::cout << "Button released: " << (int)e.cbutton.button << std::endl;
      }
    }

    // === Controller axes ===
    const int16_t leftX =
        SDL_GameControllerGetAxis(controller, SDL_CONTROLLER_AXIS_LEFTX);
    const int16_t leftY =
        SDL_GameControllerGetAxis(controller, SDL_CONTROLLER_AXIS_LEFTY);
    const int16_t rightX =
        SDL_GameControllerGetAxis(controller, SDL_CONTROLLER_AXIS_RIGHTX);

    // Normalize to [-1, 1] with deadzone
    auto normalize = [](int16_t value) {
      return (value > 8000 || value < -8000) // deadzone
                 ? static_cast<float>(value) / 32768.0f
                 : 0.0f;
    };

    float lx = normalize(leftX);
    float ly = -normalize(leftY); // invert Y so up is +1
    float rx = normalize(rightX);

    // Stick rotation: right stick X controls yaw
    character.rotation += rx * glm::radians(180.0f) * deltaTime;
    character.rotation = fmod(character.rotation, glm::two_pi<float>());

    // Movement relative to rotation
    glm::vec3 right =
        glm::vec3(cos(character.rotation), sin(character.rotation), 0.0f);
    glm::vec3 forward =
        glm::vec3(-sin(character.rotation), cos(character.rotation), 0.0f);

    glm::vec3 velocity = (ly * forward + lx * right) * deltaTime * 2.0f;
    character.position += velocity;
  }
};
