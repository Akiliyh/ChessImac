#pragma once
#include "2DRenderer.hpp"
#include "3DRenderer.hpp"
#include "GameManager.hpp"

class Renderer {
  private:
    Renderer_2D m_renderer_2d;
    Renderer_3D m_renderer_3d;

  public:
    void draw(GameManager& game);
    Renderer() = default;
};