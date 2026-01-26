#include <imgui.h>
#include <iostream>
#include <string>
#include "Renderer.hpp"
#include "Chessboard.hpp"
#include "Pieces.hpp"

int main()
{
    float value{0.f};

    std::vector<Piece*> test_board(64, nullptr);

    Rook rook_test(20,1);

    auto moves = rook_test.get_moves(test_board);

    for (int i = 0; i < moves.size(); i++) {
        std::cout << moves[i] << " " ;
    }
    std::cout << std::endl;

    Renderer renderer{};
    Chessboard board{};
    renderer.draw(board);
}