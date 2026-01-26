#pragma once
#include <vector>

class Chessboard {
    private:
        int m_board_size {8}; // 8 so 8x8 = 64 squares
        

    public:
        // futur private
        // std::vector<Piece*> board_data // todo 
        std::vector<int> board_data {};
        void init_board();
        int get_size() const;
        Chessboard() = default;
};