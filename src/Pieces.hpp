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
        Bishop(const int position, const bool white);
        std::vector<int> get_moves(std::vector<Piece*>& board);

        void get_moves_diag (const int delta, const std::vector<Piece*>& board, std::vector<int>& moves);

};

class Knight : public Piece {
    public:
        Knight(const int position, const bool white);
        std::vector<int> get_moves(std::vector<Piece*>& board);
};