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
    int        m_position;
    bool       on_focus; // we wanna display possible moves only when clicked
    char       m_label;
    bool       first_move{true};
    PieceColor m_color;

    Piece(const int x, const int y, PieceColor color)
        : m_position(at(x, y)), m_color(color), on_focus(false), m_label{}
    {}

    Piece(const std::string& alg_notation, PieceColor color)
        : m_position(at(alg_notation)), m_color(color), on_focus(false), m_label{}
    {}

    Piece(const Piece&)            = default; // copy constructor
    Piece& operator=(const Piece&) = default; // copy assignment
    Piece(Piece&&)                 = default; // move constructor
    Piece& operator=(Piece&&)      = default; // move assignment
    virtual ~Piece()               = default; // destructor

    virtual std::vector<int> get_moves(std::vector<Piece*>& board) = 0;
    int                      movement();

    static int at(int x, int y)
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
    Rook(int x, int y, PieceColor color);
    Rook(const std::string& alg_notation, PieceColor color);
    std::vector<int> get_moves(std::vector<Piece*>& board) override;
};

class Bishop : public Piece {
  public:
    Bishop(int x, int y, PieceColor color);
    Bishop(const std::string& alg_notation, PieceColor color);
    std::vector<int> get_moves(std::vector<Piece*>& board) override;
};

class Queen : public Piece {
  public:
    Queen(int x, int y, PieceColor color);
    Queen(const std::string& alg_notation, PieceColor color);
    std::vector<int> get_moves(std::vector<Piece*>& board) override;
};

class King : public Piece {
  public:
    King(int x, int y, PieceColor color);
    King(const std::string& alg_notation, PieceColor color);
    std::vector<int> get_moves(std::vector<Piece*>& board) override;
};

class Knight : public Piece {
  public:
    Knight(int x, int y, PieceColor color);
    Knight(const std::string& alg_notation, PieceColor color);
    std::vector<int> get_moves(std::vector<Piece*>& board) override;
};

class Pawn : public Piece {
  public:
    Pawn(int x, int y, PieceColor color);
    Pawn(const std::string& alg_notation, PieceColor color);
    std::vector<int> get_moves(std::vector<Piece*>& board) override;
};