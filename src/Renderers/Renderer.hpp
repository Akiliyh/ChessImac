#pragma once
#include "2DRenderer.hpp"
#include "3DRenderer.hpp"
#include "GameManager.hpp"

class Renderer {
  private:
    GameState m_current_state = GameState::MainMenu;
    Renderer_2D renderer_2d;
    Renderer_3D renderer_3d;

  public:
    void draw(GameManager& game);
    void drawStart(GameManager& game);
    Renderer() = default;
};