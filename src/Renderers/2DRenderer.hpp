#pragma once
#include "GameManager.hpp"

class Renderer_2D {
  public:
    void draw(GameManager& game);
    Renderer_2D() = default;
};