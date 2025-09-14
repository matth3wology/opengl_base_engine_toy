#pragma once

#include "Mesh.hpp"
#include "ShaderPipeline.hpp"

#include <glm/gtc/matrix_transform.hpp>

class Character {
public:
  std::shared_ptr<Mesh> mesh;
  glm::vec3 position;
  float rotation = 0.0f;

  Character(std::shared_ptr<Mesh> mesh = nullptr) : mesh(mesh) {}

  virtual ~Character() = default;

  void render(ShaderPipeline &shader) {
    if (mesh) {
      glm::mat4 model = glm::mat4(1.0);
      model = glm::translate(model, position);
      model = glm::rotate(model, rotation, glm::vec3(0.0, 0.0, 1.0));
      shader.useMat4("transformation", model);
      mesh->draw();
    }
  }
};
