#pragma once
#include "Chessboard.hpp"

class Chessboard;

class Renderer {

    public:
        void draw(Chessboard& board);
        Renderer() = default;
};