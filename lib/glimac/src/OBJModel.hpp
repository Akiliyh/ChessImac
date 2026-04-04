#pragma once
#include <string>
#include <vector>
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <tiny_obj_loader.h>
#include "common.hpp"

namespace glimac {

class OBJModel {
public:
    GLuint vao;
    GLuint vbo;
    int vertexCount;
    OBJModel() = default;
    ~OBJModel();

    bool load(const std::string& path);
    void draw() const;
};

}  // namespace glimac