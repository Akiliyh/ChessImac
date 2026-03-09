#include "Pieces.hpp"
#include <vector>
#include "Moves.hpp"

//=============================================================================
// PIECE FUNCTIONS
//=============================================================================

int Piece::get_position() const
{
    return m_position;
}

void Piece::update_position(int position)
{
    this->m_position = position;
};

char Piece::get_label() const
{
    return m_label;
}

void Piece::update_label(char label)
{
    this->m_label = label;
}

bool Piece::is_on_focus() const
{
    return on_focus;
}

void Piece::update_on_focus(bool is_on_focus)
{
    this->on_focus = is_on_focus;
}

bool Piece::is_first_move() const
{
    return first_move;
}

void Piece::update_first_move(bool is_first_move)
{
    this->first_move = is_first_move;
}

PieceColor Piece::get_color()
{
    return m_color;
}

void Piece::update_color(PieceColor color)
{
    this->m_color = color;
}

//=============================================================================
// ROOK FUNCTIONS
//=============================================================================

Rook::Rook(const int x, const int y, const PieceColor color)
    : Piece(x, y, color, color == White ? 'R' : 'r') {};

Rook::Rook(const std::string& alg_notation, const PieceColor color)
    : Piece(alg_notation, color, color == White ? 'R' : 'r') {};

std::vector<int> Rook::get_moves(std::vector<std::unique_ptr<Piece>>& board)
{
    std::vector<int> free_case;
    Moves::rook_moves(this->get_position(), this->get_color(), free_case, board);
    return free_case;
}

//=============================================================================
// BISHOP FUNCTIONS
//=============================================================================

Bishop::Bishop(const int x, const int y, const PieceColor color)
    : Piece(x, y, color, color == White ? 'B' : 'b') {};

Bishop::Bishop(const std::string& alg_notation, const PieceColor color)
    : Piece(alg_notation, color, color == White ? 'B' : 'b') {};

std::vector<int> Bishop::get_moves(std::vector<std::unique_ptr<Piece>>& board)
{
    std::vector<int> free_case;
    Moves::bishop_moves(this->get_position(), this->get_color(), free_case, board);
    return free_case;
};

//=============================================================================
// QUEEN FUNCTIONS
//=============================================================================

Queen::Queen(const int x, const int y, const PieceColor color)
    : Piece(x, y, color, color == White ? 'Q' : 'q') {};

Queen::Queen(const std::string& alg_notation, const PieceColor color)
    : Piece(alg_notation, color, color == White ? 'Q' : 'q') {};

std::vector<int> Queen::get_moves(std::vector<std::unique_ptr<Piece>>& board)
{
    std::vector<int> free_case;
    Moves::queen_moves(this->get_position(), this->get_color(), free_case, board);
    return free_case;
};

//=============================================================================
// KNIGHT FUNCTIONS
//=============================================================================

Knight::Knight(const int x, const int y, const PieceColor color)
    : Piece(x, y, color, color == White ? 'N' : 'n') {};

Knight::Knight(const std::string& alg_notation, const PieceColor color)
    : Piece(alg_notation, color, color == White ? 'N' : 'n') {};

std::vector<int> Knight::get_moves(std::vector<std::unique_ptr<Piece>>& board)
{
    std::vector<int> free_case;
    Moves::knight_moves(this->get_position(), this->get_color(), free_case, board);
    return free_case;
}

//=============================================================================
// KING FUNCTIONS
//=============================================================================

King::King(const int x, const int y, const PieceColor color)
    : Piece(x, y, color, color == White ? 'K' : 'k') {};

King::King(const std::string& alg_notation, const PieceColor color)
    : Piece(alg_notation, color, color == White ? 'K' : 'k') {};

std::vector<int> King::get_moves(std::vector<std::unique_ptr<Piece>>& board)
{
    std::vector<int> free_case;
    Moves::king_moves(this->get_position(), this->get_color(), free_case, board);
    return free_case;
}

//=============================================================================
// PAWN FUNCTIONS
//=============================================================================

Pawn::Pawn(const int x, const int y, const PieceColor color)
    : Piece(x, y, color, color == White ? 'P' : 'p') {};

Pawn::Pawn(const std::string& alg_notation, const PieceColor color)
    : Piece(alg_notation, color, color == White ? 'P' : 'p') {};

std::vector<int> Pawn::get_moves(std::vector<std::unique_ptr<Piece>>& board)
{
    std::vector<int> free_case;
    Moves::pawn_moves(
        this->get_position(), this->get_color(), free_case, board, this->is_first_move()
    );
    return free_case;
};