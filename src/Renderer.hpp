#pragma once
#include "Chessboard.hpp"
#include "GameManager.hpp"

class Chessboard;

class Renderer {
  public:
    void draw(GameManager& game);
    void display_possible_moves(
        Chessboard& board, Piece* current_square, Piece* previous_square,
        std::vector<int>& possible_moves
    );
    Renderer() = default;
};