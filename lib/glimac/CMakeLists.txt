cmake_minimum_required(VERSION 3.20)
project(glimac)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

file(GLOB_RECURSE SRC_FILES src/*.cpp src/*.hpp)

# Add cpp files
# add_library(stb_image third-party/stb_image.cpp)
add_library(glimac ${SRC_FILES})

# Add hpp files
target_include_directories(glimac PUBLIC src third-party)
# target_link_libraries(glimac PRIVATE stb_image)

# ---Add glm---
# target_include_directories(glfeur PUBLIC third-party/glm)
add_subdirectory(third-party/glm)
target_link_libraries(glimac PUBLIC glm)

# Add quick_imgui for glad
target_link_libraries(glimac PUBLIC quick_imgui)

target_compile_definitions(${PROJECT_NAME} PRIVATE SHADER_DIR="${CMAKE_SOURCE_DIR}/shaders")