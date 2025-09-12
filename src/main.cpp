#include "glm/ext/matrix_float4x4.hpp"
#include "glm/ext/matrix_transform.hpp"
#include "glm/ext/vector_float3.hpp"
#include <cmath>
#include <glad/glad.h>

#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>
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

class ShaderPipeline {
  unsigned int shaderProgram;

public:
  ShaderPipeline(const char *vtxShader, const char *fmntShader) {

    // Vertex shader
    unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vtxShader, nullptr);
    glCompileShader(vertexShader);

    // Fragment shader
    unsigned int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fmntShader, nullptr);
    glCompileShader(fragmentShader);

    // Shader program
    shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
  }

  ~ShaderPipeline() { glDeleteProgram(this->shaderProgram); }

  void use() { glUseProgram(shaderProgram); }

  void useMat4(const std::string &name, glm::mat4 &input) {
    unsigned int location = glGetUniformLocation(shaderProgram, name.c_str());
    glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(input));
  }
};

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

class Hero : public Character {
public:
  Hero(std::shared_ptr<Mesh> meshPtr) : Character(meshPtr) {}
};

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

    hero = std::make_unique<Hero>(MeshFactory::createPyramid());
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

  void update(float deltaTime) {
    float dx = 0.0f, dy = 0.0f;

    // WASD controls
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
      dy += 1.0f;
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
      dy -= 1.0f;
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
      dx -= 1.0f;
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
      dx += 1.0f;
    if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS) // rotate left
      hero->rotation += glm::radians(90.0f) * deltaTime;
    hero->rotation = fmod(hero->rotation, glm::two_pi<float>());
    if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS) // rotate right
      hero->rotation -= glm::radians(90.0f) * deltaTime;
    hero->rotation = fmod(hero->rotation, glm::two_pi<float>());

    // Move hero relative to current position
    // hero->rotation += fmod(dx, 180);
    glm::vec3 right = glm::vec3(cos(hero->rotation), sin(hero->rotation), 0.0f);
    glm::vec3 forward =
        glm::vec3(-sin(hero->rotation), cos(hero->rotation), 0.0f);

    glm::vec3 velocity = (dy * forward + dx * right) * deltaTime * 2.0f;
    hero->position += velocity;
  }
};

int main() {

  GEngine gameEngine = GEngine();

  float lastTime = glfwGetTime();
  while (!glfwWindowShouldClose(gameEngine.get_window())) {
    float currentTime = glfwGetTime();
    float deltaTime = currentTime - lastTime;
    lastTime = currentTime;

    gameEngine.update(deltaTime);
    gameEngine.render();

    glfwSwapBuffers(gameEngine.get_window());
    glfwPollEvents();
  }

  return 0;
}
