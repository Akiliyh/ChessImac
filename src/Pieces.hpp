#pragma once
#include <cstdint>
#include <cstdlib>
#include <string>
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
        bool first_move {true};

        
        Piece(const int x, const int y, PieceColor color)
        : m_position(at(x,y)), m_color(color), on_focus(false), m_label{} {}

        Piece(const std::string& alg_notation, PieceColor color)
        : m_position(at(alg_notation)), m_color(color), on_focus(false), m_label{} {}
        
        virtual std::vector<int> get_moves(std::vector<Piece*>& board) = 0;
        int movement();

        int at(int x, int y)
        {
            return x + y * 8;
        }

        // NOTE FOR OURSELVES SOURCE OF TRUTH HERE WE WANT TO HAVE THE BOARDSIZE TOO

        int at(const std::string& alg_notation)
        {
            const char base_c = 'a';
            int new_x = alg_notation[0] - base_c;
            int new_y = 7 - (alg_notation[1] - '1');
            return new_x + new_y * 8;
        }

        // I THINK I NEED TO ADD A CONDITION IF PIECE COLOR = PLAYER COLOR
};

class Rook : public Piece {
    public:
        Rook(const int x, const int y, const PieceColor color);
        Rook(const std::string& alg_notation, const PieceColor color);
        std::vector<int> get_moves(std::vector<Piece*>& board) override;
};

class Bishop : public Piece {
    public:
        Bishop(const int x, const int y, const PieceColor color);
        Bishop(const std::string& alg_notation, const PieceColor color);
        std::vector<int> get_moves(std::vector<Piece*>& board) override;
};

class Queen : public Piece {
   public:
        Queen(const int x, const int y, const PieceColor color);
        Queen(const std::string& alg_notation, const PieceColor color);
        std::vector<int> get_moves(std::vector<Piece*>& board) override;
};

class King : public Piece {
    public:
        King(const int x, const int y, const PieceColor color);
        King(const std::string& alg_notation, const PieceColor color);
        std::vector<int> get_moves(std::vector<Piece*>& board) override;
};

class Knight : public Piece {
    public:
        Knight(const int x, const int y, const PieceColor color);
        Knight(const std::string& alg_notation, const PieceColor color);
        std::vector<int> get_moves(std::vector<Piece*>& board) override;
};

class Pawn : public Piece {
    public:
        Pawn(const int x, const int y, const PieceColor color);
        Pawn(const std::string& alg_notation, const PieceColor color);
        std::vector<int> get_moves(std::vector<Piece*>& board) override;
};

struct Moves {
    public:
        static void get_moves_vertical(const int m_position, const bool m_white, const int delta, const std::vector<Piece*>& board, std::vector<int>& moves);
        static void get_moves_horizontal(const int m_position, const bool m_white, const int delta, const std::vector<Piece*>& board, std::vector<int>& moves);
        static void get_moves_diag (const int m_position, const bool m_white,const int delta, const std::vector<Piece*>& board, std::vector<int>& moves);

};