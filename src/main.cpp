#include <imgui.h>
#include <iostream>
#include <string>
#include "Renderer.hpp"
#include "Chessboard.hpp"
#include "Pieces.hpp"

int main()
{
    Renderer renderer{};
    Chessboard board{};

    int position_test {24};

    Rook rook_test(position_test+1,White);
    Bishop bishop_test(position_test,White);
    Queen queen_test(position_test+2,White);
    King king_test(position_test+3,White);
    Knight knight_test(position_test+4,Black);

    auto moves = rook_test.get_moves(board.board_data);
    auto bishop_moves = bishop_test.get_moves(board.board_data);
    auto queen_moves = queen_test.get_moves(board.board_data);
    auto king_moves = king_test.get_moves(board.board_data);
    auto knight_moves = knight_test.get_moves(board.board_data);

    for (int i = 0; i < moves.size(); i++) {
        std::cout << moves[i] << " " ;
    }
    std::cout << std::endl;

    for (int i = 0; i < bishop_moves.size(); i++) {
        std::cout << bishop_moves[i] << " " ;
    }
    std::cout << std::endl;

    for (int i = 0; i < queen_moves.size(); i++) {
        std::cout << queen_moves[i] << " " ;
    }
    std::cout << std::endl;

    for (int i = 0; i < knight_moves.size(); i++) {
        std::cout << knight_moves[i] << " " ;
    }
    std::cout << std::endl;

    for (int i = 0; i < king_moves.size(); i++) {
        std::cout << king_moves[i] << " " ;
    }
    std::cout << std::endl;

    // here we assign the position of the pieces to the board
    board.board_data[rook_test.m_position] = (&rook_test);
    board.board_data[bishop_test.m_position] = (&bishop_test);
    board.board_data[queen_test.m_position] = (&queen_test);
    board.board_data[knight_test.m_position] = (&knight_test);

    board.move_piece(&rook_test, 9);
    board.move_piece(&queen_test, 24);

    renderer.draw(board);
}