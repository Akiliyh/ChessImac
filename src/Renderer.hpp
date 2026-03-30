#pragma once
#include "GameManager.hpp"

class Renderer {
  private:
    GameState m_current_state = GameState::MainMenu;

  public:
    void draw(GameManager& game);
    void drawStart(GameManager& game);
    Renderer() = default;
};