#pragma once
#include "Chessboard.hpp"
#include "GameManager.hpp"

class Chessboard;

class Renderer {
  public:
    void draw(GameManager& game);
    Renderer() = default;
};