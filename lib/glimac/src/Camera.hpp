#pragma once
#include <cmath>
#include <vector>
#include "glm.hpp"

namespace glimac {

class Camera {
  public:
    virtual ~Camera() = default;

    virtual void moveFront(float delta) = 0;
    virtual glm::vec3 get_position() = 0;
    virtual void rotateLeft(float degrees) = 0;
    virtual void rotateUp(float degrees) = 0;

    virtual glm::mat4 getViewMatrix() const = 0;
};

} // namespace glimac