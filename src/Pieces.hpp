#pragma once
#include <vector>

class Piece {
    public:
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
};

class Queen : public Piece {
   public:
        Queen(const int position, const bool white);
        std::vector<int> get_moves(std::vector<Piece*>& board);
};

class Knight : public Piece {
    public:
        Knight(const int position, const bool white);
        std::vector<int> get_moves(std::vector<Piece*>& board);
};

struct Moves {
    public:
        static void get_moves_vertical(const int m_position, const bool m_white, const int delta, const std::vector<Piece*>& board, std::vector<int>& moves);
        static void get_moves_horizontal(const int m_position, const bool m_white, const int delta, const std::vector<Piece*>& board, std::vector<int>& moves);
        static void get_moves_diag (const int m_position, const bool m_white,const int delta, const std::vector<Piece*>& board, std::vector<int>& moves);

};