#pragma once
#include <vector>

class Piece {
    public:
    //futur private
        int m_position;
        bool m_white;
        
        std::vector<std::vector<int>> get_moves();
        int mouvement();
};

class Rook : public Piece {
    public:
        Rook(const int position, const bool white);
        std::vector<int> get_moves(std::vector<Piece*>& board);
};

class Bishop : public Piece {
    public:
        std::vector<std::vector<int>> get_moves(std::vector<Piece*>& board);
};