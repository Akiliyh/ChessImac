#include "Chessboard.hpp"
#include <vector>

void Chessboard::init_board() {
    // do smth here;
}

int Chessboard::get_size() const{
    return m_board_size;
}

Chessboard::Chessboard(): board_data(64, nullptr) {

}