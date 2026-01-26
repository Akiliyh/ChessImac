#pragma once
#include <vector>
#include "Pieces.hpp"

class Chessboard {
    private:
        int m_board_size {8}; // 8 so 8x8 = 64 squares
        

    public:
        // futur private
        std::vector<Piece*> board_data {};
        void init_board();
        int get_size() const;
        Chessboard();
};