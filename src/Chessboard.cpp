#include "Chessboard.hpp"
#include <vector>
#include <iostream>
#include <algorithm>
#include "Pieces.hpp"

void Chessboard::init_board() {
    // do smth here;
}

int Chessboard::get_size() const{
    return m_board_size;
}

Chessboard::Chessboard(): board_data(64, nullptr) {

}

void Chessboard::move_piece(Piece* active_square, int dest_position) {
    std::vector<int> legal_moves = active_square->get_moves(this->board_data);

    // we check if the move is legal
    if (std::find(legal_moves.begin(), legal_moves.end(), dest_position) != legal_moves.end()) {

        this->board_data[active_square->m_position] = (nullptr);
        this->board_data[dest_position] = (active_square);
        active_square->m_position = dest_position;

        if (active_square->first_move) {
            active_square->first_move = false;
        }
    } else {
        std::cout << "Illegal move!" << std::endl;
    }
}