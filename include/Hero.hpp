#pragma once

#include "Character.hpp"
#include "Controller.hpp"

class Hero : public Character {
public:
  std::unique_ptr<Controller> controller;

  Hero(std::shared_ptr<Mesh> meshPtr, std::unique_ptr<Controller> ctrl)
      : Character(meshPtr), controller(std::move(ctrl)) {}

  void update(GLFWwindow *window, float deltaTime) {
    if (controller)
      controller->update(*this, window, deltaTime);
  }
};
