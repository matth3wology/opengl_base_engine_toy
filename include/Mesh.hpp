#pragma once

#include <glad/glad.h>
#include <memory>
#include <vector>

class Mesh {
public:
  GLuint VAO, VBO, EBO;
  std::vector<float> vertices;
  std::vector<unsigned int> indices;

  Mesh(const std::vector<float> &verts, const std::vector<unsigned int> inds)
      : vertices(verts), indices(inds) {
    setupMesh();
  }

  ~Mesh() {
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);
  }

  void setupMesh() {
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float),
                 vertices.data(), GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int),
                 indices.data(), GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float),
                          (void *)0);

    glEnableVertexAttribArray(0);

    glBindVertexArray(0);
  }

  void draw() const {
    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
  }
};

class MeshFactory {
public:
  static std::shared_ptr<Mesh> createCube() {
    std::vector<float> vertices = {
        // Front face
        -0.5f, -0.5f, 0.5f, 1.0f, 1.0f, 1.0f, 0.5f, -0.5f, 0.5f, 1.0f, 1.0f,
        1.0f, 0.5f, 0.5f, 0.5f, 1.0f, 1.0f, 1.0f, -0.5f, 0.5f, 0.5f, 1.0f, 1.0f,
        1.0f,
        // Back face
        -0.5f, -0.5f, -0.5f, 1.0f, 1.0f, 1.0f, 0.5f, -0.5f, -0.5f, 1.0f, 1.0f,
        1.0f, 0.5f, 0.5f, -0.5f, 1.0f, 1.0f, 1.0f, -0.5f, 0.5f, -0.5f, 1.0f,
        1.0f, 1.0f};

    std::vector<unsigned int> indices = {
        0, 1, 2, 2, 3, 0, // Front
        4, 5, 6, 6, 7, 4, // Back
        7, 3, 0, 0, 4, 7, // Left
        1, 5, 6, 6, 2, 1, // Right
        3, 2, 6, 6, 7, 3, // Top
        0, 1, 5, 5, 4, 0  // Bottom
    };

    return std::make_shared<Mesh>(vertices, indices);
  }

  static std::shared_ptr<Mesh> createPyramid() {
    std::vector<float> vertices = {// Base vertices
                                   -0.5f, -0.5f, -0.5f, 1.0f, 1.0f, 1.0f, 0.5f,
                                   -0.5f, -0.5f, 1.0f, 1.0f, 1.0f, 0.5f, -0.5f,
                                   0.5f, 1.0f, 1.0f, 1.0f, -0.5f, -0.5f, 0.5f,
                                   1.0f, 1.0f, 1.0f,
                                   // Apex
                                   0.0f, 0.5f, 0.0f, 1.0f, 1.0f, 1.0f};

    std::vector<unsigned int> indices = {
        // Base
        0, 1, 2, 2, 3, 0,
        // Sides
        0, 1, 4, // Front
        1, 2, 4, // Right
        2, 3, 4, // Back
        3, 0, 4  // Left
    };

    return std::make_shared<Mesh>(vertices, indices);
  }
};
