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

    board.load_board_from_fen("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1");

    Rook rook_test(0,0,White);
    Bishop bishop_test(1,0,White);
    Queen queen_test(4,5,White);
    King king_test(1,1,Black);
    Knight knight_test(2,1,Black);
    Pawn pawn_test("g3",White);
    Pawn pawn_test2("d2",White);
    Pawn pawn_test3("d7",Black);

    auto moves = rook_test.get_moves(board.board_data);
    auto bishop_moves = bishop_test.get_moves(board.board_data);
    auto queen_moves = queen_test.get_moves(board.board_data);
    auto king_moves = king_test.get_moves(board.board_data);
    auto knight_moves = knight_test.get_moves(board.board_data);
    auto pawn_moves = pawn_test.get_moves(board.board_data);
    auto pawn_moves2 = pawn_test2.get_moves(board.board_data);
    auto pawn_moves3 = pawn_test3.get_moves(board.board_data);

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

    for (int i = 0; i < pawn_moves.size(); i++) {
        std::cout << pawn_moves[i] << " " ;
    }
    std::cout << std::endl;
    
    for (int i = 0; i < pawn_moves2.size(); i++) {
        std::cout << pawn_moves2[i] << " " ;
    }
    std::cout << std::endl;

    for (int i = 0; i < pawn_moves3.size(); i++) {
        std::cout << pawn_moves3[i] << " " ;
    }
    std::cout << std::endl;

    // here we assign the position of the pieces to the board
    board.board_data[rook_test.m_position] = (&rook_test);
    board.board_data[bishop_test.m_position] = (&bishop_test);
    board.board_data[queen_test.m_position] = (&queen_test);
    board.board_data[knight_test.m_position] = (&knight_test);
    board.board_data[king_test.m_position] = (&king_test);
    board.board_data[pawn_test.m_position] = (&pawn_test);
    board.board_data[pawn_test2.m_position] = (&pawn_test2);
    board.board_data[pawn_test3.m_position] = (&pawn_test3);

    // board.move_piece(&rook_test, 9);
    // board.move_piece(&queen_test, 24);
    board.move_piece(&pawn_test3, 27);

    renderer.draw(board);
}