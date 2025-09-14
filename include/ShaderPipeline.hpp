#pragma once

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <string>

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
