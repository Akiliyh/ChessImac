#pragma once
#include "Renderer.hpp"
#include "GameManager.hpp"

class Renderer_3D : Renderer {
  public:
    int init(GameManager& game);
    int draw(GameManager& game);
    Renderer_3D() = default;
};