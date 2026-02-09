#pragma once
#include "Chessboard.hpp"

class Chessboard;

class Renderer {

    public:
        void draw(Chessboard& board);
        void display_possible_moves(Chessboard& board, Piece* current_square, Piece* previous_square, std::vector<int> &possible_moves);
        Renderer() = default;
};