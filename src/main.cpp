
#define SDL_MAIN_HANDLED

#include "GameEngine.hpp"

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
