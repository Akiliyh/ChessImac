#pragma once
#include "GameManager.hpp"

class Renderer_2D {
  private:
    GameState m_current_state = GameState::MainMenu;

  public:
    void draw(GameManager& game);
    Renderer_2D() = default;
};