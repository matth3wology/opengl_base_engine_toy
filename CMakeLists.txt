cmake_minimum_required(VERSION 3.16)
project(app VERSION 1.0.0 LANGUAGES C CXX)

include(FetchContent)
set(CMAKE_POLICY_VERSION_MINIMUM 3.5)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# -----------------------------
# Output directory for executable and DLLs
# -----------------------------
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_DEBUG ${CMAKE_BINARY_DIR}/debug)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELEASE ${CMAKE_BINARY_DIR}/release)

# -----------------------------
# Fetch GLFW (for windowing + OpenGL context)
# -----------------------------
FetchContent_Declare(
  glfw
  GIT_REPOSITORY https://github.com/glfw/glfw.git
  GIT_TAG 3.4
)
set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_SHARED_LIBS OFF CACHE BOOL "" FORCE)

FetchContent_MakeAvailable(glfw)

# Fetch SDL2
FetchContent_Declare(
  SDL2
  GIT_REPOSITORY https://github.com/libsdl-org/SDL.git
  GIT_TAG release-2.30.2  # pick a stable release
)
FetchContent_MakeAvailable(SDL2)

# -----------------------------
# -----------------------------
# Fetch GLAD (for OpenGL function loading)
# -----------------------------
FetchContent_Declare(
    glad
    GIT_REPOSITORY https://github.com/Dav1dde/glad.git
    GIT_TAG v0.1.36
)

FetchContent_MakeAvailable(glad)

# -----------------------------
# Fetch GLM (for OpenGL math functions)
# -----------------------------
FetchContent_Declare(
    glm
    GIT_REPOSITORY https://github.com/g-truc/glm.git
    GIT_TAG 1.0.1
)

FetchContent_MakeAvailable(glm)

# -----------------------------
# Set build type
# -----------------------------
if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release)
endif()

# -----------------------------
# Find required packages
# -----------------------------
find_package(OpenGL REQUIRED)

# -----------------------------
# Source files
# -----------------------------
set(SOURCES
    src/main.cpp
)

# -----------------------------
# Create executable
# -----------------------------
add_executable(${PROJECT_NAME} ${SOURCES})

# -----------------------------
# Include directories
# -----------------------------
target_include_directories(${PROJECT_NAME} PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)

# -----------------------------
# Link libraries
# -----------------------------
target_link_libraries(${PROJECT_NAME} PRIVATE
    OpenGL::GL
    glfw
    glad
    glm
    SDL2::SDL2 
    SDL2::SDL2main
)

