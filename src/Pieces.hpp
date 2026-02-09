#pragma once
#include <cstdint>
#include <vector>

enum PieceColor : uint8_t {
    White,
    Black,
};
class Piece {
    public:
        int m_position;
        bool on_focus; // we wanna display possible moves only when clicked
        char m_label;
        PieceColor m_color;
        
        Piece(int position, PieceColor color)
        : m_position(position), m_color(color), on_focus(false), m_label{} {}
        
        virtual std::vector<int> get_moves(std::vector<Piece*>& board) = 0;
        int movement();
};

class Rook : public Piece {
    public:
        Rook(const int position, const PieceColor color);
        std::vector<int> get_moves(std::vector<Piece*>& board) override;
};

class Bishop : public Piece {
    public:
        Bishop(const int position, const PieceColor color);
        std::vector<int> get_moves(std::vector<Piece*>& board) override;
};

class Queen : public Piece {
   public:
        Queen(const int position, const PieceColor color);
        std::vector<int> get_moves(std::vector<Piece*>& board) override;
};

class King : public Piece {
    public:
        King(const int position, const PieceColor color);
        std::vector<int> get_moves(std::vector<Piece*>& board) override;
};

class Knight : public Piece {
    public:
        Knight(const int position, const PieceColor color);
        std::vector<int> get_moves(std::vector<Piece*>& board) override;
};

struct Moves {
    public:
        static void get_moves_vertical(const int m_position, const bool m_white, const int delta, const std::vector<Piece*>& board, std::vector<int>& moves);
        static void get_moves_horizontal(const int m_position, const bool m_white, const int delta, const std::vector<Piece*>& board, std::vector<int>& moves);
        static void get_moves_diag (const int m_position, const bool m_white,const int delta, const std::vector<Piece*>& board, std::vector<int>& moves);

};