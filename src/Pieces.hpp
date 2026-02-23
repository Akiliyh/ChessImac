#pragma once
#include <cstdint>
#include <cstdlib>
#include <memory>
#include <string>
#include <vector>

enum PieceColor : uint8_t {
    White,
    Black,
};
class Piece {
  private:
    int        m_position;
    char       m_label;
    bool       on_focus; // we wanna display possible moves only when clicked
    bool       first_move{true};
    PieceColor m_color;

  public:
    Piece(const int x, const int y, PieceColor color, char m_label)
        : m_position(at(x, y)), m_color(color), on_focus(false), m_label(m_label)
    {}

    Piece(const std::string& alg_notation, PieceColor color, char m_label)
        : m_position(at(alg_notation)), m_color(color), on_focus(false), m_label(m_label)
    {}

    Piece(const Piece&)            = default; // copy constructor
    Piece& operator=(const Piece&) = default; // copy assignment
    Piece(Piece&&)                 = default; // move constructor
    Piece& operator=(Piece&&)      = default; // move assignment
    virtual ~Piece()               = default; // destructor

    int        get_position() const;
    char       get_label() const;
    bool       is_on_focus() const;
    bool       is_first_move() const;
    PieceColor get_color();
    int        movement();

    void update_position(int position);
    void update_label(char label);
    void update_on_focus(bool is_on_focus);
    void update_first_move(bool is_first_move);
    void update_color(PieceColor color);

    virtual std::vector<int> get_moves(std::vector<std::unique_ptr<Piece>>& board) = 0;

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
    std::vector<int> get_moves(std::vector<std::unique_ptr<Piece>>& board);
};

class Bishop : public Piece {
  public:
    Bishop(int x, int y, PieceColor color);
    Bishop(const std::string& alg_notation, PieceColor color);
    std::vector<int> get_moves(std::vector<std::unique_ptr<Piece>>& board);
};

class Queen : public Piece {
  public:
    Queen(int x, int y, PieceColor color);
    Queen(const std::string& alg_notation, PieceColor color);
    std::vector<int> get_moves(std::vector<std::unique_ptr<Piece>>& board);
};

class King : public Piece {
  public:
    King(int x, int y, PieceColor color);
    King(const std::string& alg_notation, PieceColor color);
    std::vector<int> get_moves(std::vector<std::unique_ptr<Piece>>& board);
};

class Knight : public Piece {
  public:
    Knight(int x, int y, PieceColor color);
    Knight(const std::string& alg_notation, PieceColor color);
    std::vector<int> get_moves(std::vector<std::unique_ptr<Piece>>& board);
};

class Pawn : public Piece {
  public:
    Pawn(int x, int y, PieceColor color);
    Pawn(const std::string& alg_notation, PieceColor color);
    std::vector<int> get_moves(std::vector<std::unique_ptr<Piece>>& board);
};